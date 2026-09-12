#include "helper.hpp"

#include <safetyhook.hpp>
#include <Zydis.h>
#include <bcrypt.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <atomic>
#include <array>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace
{
    // Global integration copy; stable gameplay source remains untouched.

    // This sequence identifies the complete camera-view copy path, not merely a
    // generic MOVSS instruction. The JNZ displacement may change between builds.
    constexpr std::uint8_t kCameraWriterSignature[] = {
        0xF6, 0x86, 0x62, 0x02, 0x00, 0x00, 0x10,
        0xF3, 0x0F, 0x10, 0x86, 0x30, 0x02, 0x00, 0x00,
        0x0F, 0x85, 0x00, 0x00, 0x00, 0x00,
        0x48, 0x8D, 0x4B, 0x30,
        0xF3, 0x0F, 0x11, 0x43, 0x30,
        0xF3, 0x0F, 0x10, 0x86, 0x54, 0x02, 0x00, 0x00,
        0xF3, 0x0F, 0x11, 0x43, 0x5C,
        0x0F, 0xB6, 0x96, 0x59, 0x02, 0x00, 0x00,
        0x8B, 0x43, 0x68, 0x83, 0xE2, 0x01, 0x83, 0xE0, 0xFE,
        0x09, 0xD0, 0x89, 0x43, 0x68,
        0x0F, 0xB6, 0x96, 0x59, 0x02, 0x00, 0x00,
        0x83, 0xE2, 0x04, 0x83, 0xE0, 0xFB, 0x09, 0xD0, 0x89,
        0x43, 0x68, 0x8A, 0x96, 0x63, 0x02, 0x00, 0x00, 0x88,
        0x53, 0x6C,
    };
    constexpr std::size_t kFovWriteOffsetInSignature = 25;
    constexpr std::uintptr_t kAspectOffset = 0x254;
    constexpr std::uintptr_t kFlagsOffset = 0x259;
    constexpr float kWideAspect = 32.0f / 9.0f;
    // Canonical PC 21:9 framing used by the validated 3440x1440 profile.
    constexpr float kCinemaAspect = 3440.0f / 1440.0f;
    constexpr float kNativeAspect = 16.0f / 9.0f;
    constexpr std::uint8_t kCinematicStorePrefix[] = { 0xC7, 0x80, 0x54, 0x02, 0x00, 0x00 };
    constexpr std::uint8_t kCinematicOriginalImmediate[] = { 0x39, 0x8E, 0xE3, 0x3F };
    constexpr std::size_t kCinematicImmediateOffset = 6;
    constexpr std::size_t kCinematicStoreInstructionLength = 10;
    constexpr float kRecoveryEpsilon = 0.01f;
    constexpr char kDialogueBoundarySignature[] =
        "48 8B 0A 48 8B 01 0F 28 CE FF 90 08 06 00 00 "
        "0F 2E 76 2C 75 02 7B 1A";
    constexpr std::size_t kDialogueBoundaryHookOffset = 9;
    constexpr float kDialogueRecoveryEpsilon = 1.0f;
    constexpr float kDialogueTransformEpsilon = 0.01f;
    constexpr float kDialogueContextFovJump = 5.0f;

    // These signatures describe the 2.0.3 -> 2.0.4 transition topology, not
    // fixed addresses. Relative call displacements and the ENTER RIP-relative
    // scalar displacement are intentionally wildcarded.
    constexpr char kCinematicAspectSetterSignature[] =
        "48 8B 41 18 48 8B 88 F8 00 00 00 80 89 59 02 00 00 01 "
        "48 8B 80 F8 00 00 00 C7 80 54 02 00 00 39 8E E3 3F B0 01 C3";
    constexpr char kCinematicEnterSignature[] =
        "F3 0F 10 05 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 89 F1 E8 ?? ?? ?? ?? "
        "48 89 C1 31 D2 E8 ?? ?? ?? ??";
    constexpr char kCinematicExitSignature[] =
        "F3 0F 10 47 38 E8 ?? ?? ?? ?? 48 89 F1 E8 ?? ?? ?? ?? "
        "48 89 C1 31 D2 E8 ?? ?? ?? ??";
    // Current post-update EXIT topology: the FOV sample is loaded through an
    // indexed source expression before entering the same consumer chain.
    constexpr char kCinematicExitIndexedSignature[] =
        "40 0F B6 C7 F3 0F 10 44 83 38 E8 ?? ?? ?? ?? "
        "48 89 F1 E8 ?? ?? ?? ?? 48 89 C1 31 D2 E8 ?? ?? ?? ?? "
        "48 85 C0 74 ?? 48 89 C7 48 8B 00 48 89 F9 "
        "FF 90 80 08 00 00 48 8B 07 48 89 F9 FF 90 68 08 00 00";
    constexpr std::uint8_t kEnterVcallPair[] = {
        0xFF, 0x90, 0x78, 0x08, 0x00, 0x00, 0x48, 0x8B, 0x07, 0x48, 0x89, 0xF9,
        0xB2, 0x01, 0xFF, 0x90, 0x60, 0x08, 0x00, 0x00,
    };
    constexpr std::uint8_t kExitVcallPair[] = {
        0xFF, 0x90, 0x80, 0x08, 0x00, 0x00, 0x48, 0x8B, 0x07, 0x48, 0x89, 0xF9,
        0xFF, 0x90, 0x68, 0x08, 0x00, 0x00,
    };

    enum class CoordinatorState : std::uint32_t { Gameplay, CinematicActive, CinematicExiting };

    enum class ReplayState : std::uint32_t { WaitingForAutomaticUpdate, AppliedConstrainPass, Complete };

    enum class CinematicAspectPolicy : std::uint32_t { Auto, Native, Forced16x9, Forced21x9, Forced32x9 };

    enum class DialogueZoomPolicy : std::uint32_t { Native, Adaptive, Reduced, Disabled };
    enum class DialoguePhase : std::uint32_t { Inactive, Candidate, Active, Exiting };

    struct FeatureConfig
    {
        bool gameplayEnabled{true};
        CinematicAspectPolicy cinematicAspectPolicy{CinematicAspectPolicy::Auto};
        bool cinematicAspectPolicyExplicit{};
        DialogueZoomPolicy dialogueZoomPolicy{DialogueZoomPolicy::Reduced};
    bool hotkeysEnabled{false};
        int cinematicCycleKey{VK_F9};
        int dialogueCycleKey{VK_F10};
    };

    HMODULE g_module{};
    HMODULE g_executable = GetModuleHandle(nullptr);
    std::uint8_t* g_fovWriteAddress{};
    SafetyHookMid g_hook;
    std::shared_ptr<spdlog::logger> g_logger;
    FeatureConfig g_config{};
    std::atomic<CinematicAspectPolicy> g_runtimeCinematicPolicy{CinematicAspectPolicy::Auto};
    std::atomic<DialogueZoomPolicy> g_runtimeDialoguePolicy{DialogueZoomPolicy::Native};
    std::filesystem::path g_configPath;
    std::atomic<ReplayState> g_state{ReplayState::WaitingForAutomaticUpdate};
    std::atomic<std::uint32_t> g_lastCameraMode{};
    std::atomic<std::uint64_t> g_transitionTraceSequence{0};
    std::atomic<CoordinatorState> g_coordinator{CoordinatorState::Gameplay};
    std::atomic<bool> g_cinematicFovApplied{false};
    std::atomic<float> g_exitTargetFov{0.0f};
    std::atomic<bool> g_postExitTraceArmed{false};
    std::atomic<std::uint32_t> g_postExitTraceWriterCount{0};
    std::atomic<std::int64_t> g_postExitTraceStartNs{0};
    std::atomic<std::uint64_t> g_postExitTraceSequence{0};
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
#ifndef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS
#define POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS 120
#endif
    constexpr std::uint32_t kDeferredGameplayStableSamples = 3;
    constexpr std::int64_t kDeferredGameplayDelayNs =
        static_cast<std::int64_t>(POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS) * 1000000LL;
    std::atomic<bool> g_deferredGameplayReplayPending{false};
    std::atomic<std::uintptr_t> g_deferredGameplaySource{};
    std::atomic<std::uint32_t> g_deferredGameplayStableSamples{0};
    std::atomic<float> g_deferredGameplayLastFov{std::numeric_limits<float>::quiet_NaN()};
    std::atomic<std::int64_t> g_deferredGameplayStableSinceNs{0};
#endif
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
    std::atomic<bool> g_atomicExitHandoffPending{false};
    std::atomic<std::uintptr_t> g_atomicExitHandoffSource{};
    std::atomic<float> g_atomicExitHandoffPreviousFov{std::numeric_limits<float>::quiet_NaN()};
#endif
#ifdef POST_EXIT_CAMERA_FOV_WRITE_OWNER_TRACE
    std::atomic<std::uint32_t> g_postExitWriteOwnerHitCount{0};
#endif
#ifdef POST_EXIT_FOV_PRODUCER_STORE_TRACE
    std::atomic<std::uint32_t> g_postExitProducerStoreHitCount{0};
#endif
    SafetyHookMid g_cinematicEnterHook;
    SafetyHookMid g_cinematicExitHook;
    std::uint8_t* g_cinematicAspectStore{};
    std::uint8_t g_cinematicOriginalImmediate[sizeof(kCinematicOriginalImmediate)]{};
    bool g_cinematicAspectPatched{};
    SafetyHookMid g_cinematicAspectStoreHook;
    SafetyHookMid g_dialogueBoundaryHook;
#ifdef POST_EXIT_FOV_STATE_CONSUMER_TRACE
    SafetyHookMid g_postExitConsumerHook;
    constexpr std::uintptr_t kPostExitConsumerRva = 0x318DCD4;
    constexpr char kPostExitConsumerGameSha256[] =
        "e7b481a97c02d80581fab0bece940214a88ebe30211088a00129845a039f9293";
#endif
#ifdef POST_EXIT_CAMERA_FOV_WRITE_OWNER_TRACE
    SafetyHookMid g_postExitWriteOwnerHook;
    constexpr std::uintptr_t kPostExitWriteOwnerRva = 0x3DB2CE7;
    constexpr char kPostExitWriteOwnerGameSha256[] =
        "e7b481a97c02d80581fab0bece940214a88ebe30211088a00129845a039f9293";
#endif

#ifdef POST_EXIT_FOV_PRODUCER_STORE_TRACE
    SafetyHookMid g_postExitProducerStoreHook;
    constexpr std::uintptr_t kPostExitProducerStoreRva = 0x32B779D;
    constexpr char kPostExitProducerStoreGameSha256[] =
        "e7b481a97c02d80581fab0bece940214a88ebe30211088a00129845a039f9293";
#endif
    std::atomic<float> g_lastObservedAspect{kNativeAspect};
    std::atomic<std::uintptr_t> g_lastAutoRestoreSource{};
    std::atomic<std::uintptr_t> g_lastGameplayCameraSource{};
    std::atomic<float> g_lastGameplayCameraFov{std::numeric_limits<float>::quiet_NaN()};
    std::uintptr_t g_lastLoggedFovSource{};
    float g_lastLoggedWriterInputFov = std::numeric_limits<float>::quiet_NaN();
    float g_lastLoggedCameraWorldFov = std::numeric_limits<float>::quiet_NaN();
    float g_lastLoggedCameraFirstPersonFov = std::numeric_limits<float>::quiet_NaN();
    std::mutex g_dialogueMutex;
    DialoguePhase g_dialoguePhase{DialoguePhase::Inactive};
    float g_dialogueBaseline = std::numeric_limits<float>::quiet_NaN();
    float g_dialoguePrevious = std::numeric_limits<float>::quiet_NaN();
    float g_dialogueExitIncomingStart{std::numeric_limits<float>::quiet_NaN()};

    void ResetDialogueRuntimeState()
    {
        g_dialoguePhase = DialoguePhase::Inactive;
        g_dialogueBaseline = std::numeric_limits<float>::quiet_NaN();
        g_dialoguePrevious = std::numeric_limits<float>::quiet_NaN();
        g_dialogueExitIncomingStart = std::numeric_limits<float>::quiet_NaN();
    }
#ifdef GAMEPLAY_ONE_SHOT_CINEMATIC_TRIGGER
    std::atomic_bool g_oneShotCinematicTriggerArmed{false};
#endif
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
    std::atomic<std::uintptr_t> g_lastCameraSource{};
    std::atomic_bool g_suppressRearmAfterManualRestore{false};
    std::atomic_bool g_readyForOneShot{};
    std::atomic_bool g_readyMarkerLogged{};
    thread_local bool g_logCombinedOutputForInvocation{};

    struct CombinedOutputSnapshot
    {
        std::uintptr_t source{};
        std::uintptr_t output{};
        float sourceAspect{};
        std::uint8_t sourceFlags{};
        float outputAspect{};
        std::uint64_t outputField40{};
        std::uint64_t outputField58{};
        std::uint64_t outputField68{};
        bool valid{};
    };
    thread_local CombinedOutputSnapshot g_lastCombinedOutput{};

    struct ResolutionSnapshot
    {
        DWORD displayWidth{};
        DWORD displayHeight{};
        LONG windowWidth{};
        LONG windowHeight{};
        LONG clientWidth{};
        LONG clientHeight{};
        bool valid{};
    };
    ResolutionSnapshot g_lastResolutionSnapshot{};
#endif
#ifdef FOV_SETTINGS_TRACE_DIAGNOSTIC
    float g_lastLoggedGameplayFov = std::numeric_limits<float>::quiet_NaN();
    std::uintptr_t g_lastLoggedGameplayFovSource{};
#endif

    const char* ReplayStateName(ReplayState state)
    {
        switch (state) {
        case ReplayState::WaitingForAutomaticUpdate: return "WaitingForAutomaticUpdate";
        case ReplayState::AppliedConstrainPass: return "AppliedConstrainPass";
        case ReplayState::Complete: return "Complete";
        }
        return "Unknown";
    }

    const char* CoordinatorStateName(CoordinatorState state)
    {
        switch (state) {
        case CoordinatorState::Gameplay: return "Gameplay";
        case CoordinatorState::CinematicActive: return "CinematicActive";
        case CoordinatorState::CinematicExiting: return "CinematicExiting";
        }
        return "Unknown";
    }

    const char* DialoguePhaseName(DialoguePhase phase)
    {
        switch (phase) {
        case DialoguePhase::Inactive: return "Inactive";
        case DialoguePhase::Candidate: return "Candidate";
        case DialoguePhase::Active: return "Active";
        case DialoguePhase::Exiting: return "Exiting";
        }
        return "Unknown";
    }

    bool IsUltrawideAspect(float aspect)
    {
        return std::isfinite(aspect) && aspect > kNativeAspect + 0.001f;
    }

    bool IsValidAspect(float aspect)
    {
        return std::isfinite(aspect) && aspect > 0.0f;
    }

    template <typename... Args>
    void Log(Args&&... args)
    {
        if (!g_logger) return;
        std::ostringstream message;
        (message << ... << args);
        g_logger->info("{}", message.str());
    }

#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
    std::int64_t NowSteadyNs()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    void CancelDeferredGameplayReplay(const char* reason, std::uintptr_t source = 0,
        float fov = std::numeric_limits<float>::quiet_NaN(), float aspect = 0.0f,
        std::uint8_t flags = 0)
    {
        const bool wasPending = g_deferredGameplayReplayPending.exchange(false,
            std::memory_order_acq_rel);
        const auto samples = g_deferredGameplayStableSamples.exchange(0,
            std::memory_order_acq_rel);
        g_deferredGameplaySource.store(0, std::memory_order_release);
        g_deferredGameplayLastFov.store(std::numeric_limits<float>::quiet_NaN(),
            std::memory_order_release);
        g_deferredGameplayStableSinceNs.store(0, std::memory_order_release);
        if (wasPending || samples != 0)
            Log("DeferCancelled: reason=", reason, " source=0x", std::hex, source,
                std::dec, " fov=", fov, " aspect=", aspect, " flags=0x", std::hex,
                static_cast<unsigned>(flags), std::dec, " stableSamples=", samples, ".");
    }

    void ArmDeferredGameplayReplay(float targetFov)
    {
        g_deferredGameplaySource.store(0, std::memory_order_release);
        g_deferredGameplayStableSamples.store(0, std::memory_order_release);
        g_deferredGameplayLastFov.store(std::numeric_limits<float>::quiet_NaN(),
            std::memory_order_release);
        g_deferredGameplayStableSinceNs.store(0, std::memory_order_release);
        g_deferredGameplayReplayPending.store(true, std::memory_order_release);
        Log("DeferArmed: targetFov=", targetFov, " requiredStableSamples=3 delayMs=",
            POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS, ".");
    }
#endif

#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
    void ResetAtomicExitHandoff(const char* reason)
    {
        const bool wasPending = g_atomicExitHandoffPending.exchange(false,
            std::memory_order_acq_rel);
        g_atomicExitHandoffSource.store(0, std::memory_order_release);
        g_atomicExitHandoffPreviousFov.store(std::numeric_limits<float>::quiet_NaN(),
            std::memory_order_release);
        if (wasPending) Log("AtomicExitHandoffCancelled: reason=", reason, ".");
    }

    void ArmAtomicExitHandoff()
    {
        g_atomicExitHandoffSource.store(0, std::memory_order_release);
        g_atomicExitHandoffPreviousFov.store(std::numeric_limits<float>::quiet_NaN(),
            std::memory_order_release);
        g_atomicExitHandoffPending.store(true, std::memory_order_release);
        Log("AtomicExitHandoffArmed: trigger=first-downward-fov-sample.");
    }
#endif

    void LogGameplayFovSourceChange(std::uintptr_t source, float writerInputXmm0,
        float cameraWorldFov, float cameraFirstPersonFov, float aspect, std::uint8_t flags)
    {
        const bool sourceChanged = g_lastLoggedFovSource != source;
        const auto changed = [](float current, float previous) {
            return std::isfinite(current) != std::isfinite(previous) ||
                (std::isfinite(current) && std::isfinite(previous) &&
                    std::fabs(current - previous) > 0.01f);
        };
        if (!sourceChanged && !changed(writerInputXmm0, g_lastLoggedWriterInputFov) &&
            !changed(cameraWorldFov, g_lastLoggedCameraWorldFov) &&
            !changed(cameraFirstPersonFov, g_lastLoggedCameraFirstPersonFov)) return;

        const auto coordinator = g_coordinator.load(std::memory_order_acquire);
        DialoguePhase dialoguePhase = DialoguePhase::Inactive;
        {
            std::lock_guard dialogueLock(g_dialogueMutex);
            dialoguePhase = g_dialoguePhase;
        }
        Log("Gameplay FOV source change: event=", CoordinatorStateName(coordinator),
            " sourceChanged=", sourceChanged, " source=0x", std::hex, source, std::dec,
            " writerInputXmm0=", writerInputXmm0,
            " cameraWorldFov(+0x230)=", cameraWorldFov,
            " cameraFirstPersonFov(+0x234)=", cameraFirstPersonFov,
            " aspect=", aspect, " flags=0x", std::hex, static_cast<int>(flags), std::dec,
            " replayState=", ReplayStateName(g_state.load(std::memory_order_acquire)),
            " coordinator=", CoordinatorStateName(coordinator),
            " dialoguePhase=", DialoguePhaseName(dialoguePhase), ".");
        g_lastLoggedFovSource = source;
        g_lastLoggedWriterInputFov = writerInputXmm0;
        g_lastLoggedCameraWorldFov = cameraWorldFov;
        g_lastLoggedCameraFirstPersonFov = cameraFirstPersonFov;
    }

    template <typename T>
    bool SafeRead(std::uintptr_t address, T& value);

    bool IsExecutable(std::uintptr_t address);

    void LogPostExitWriterObservation(std::uintptr_t source, std::uintptr_t output,
        float writerInputXmm0, float cameraWorldFov, float cameraFirstPersonFov,
        float aspect, std::uint8_t flags, std::uint8_t selector,
        float outputFov, float outputAspect)
    {
        if (!g_postExitTraceArmed.load(std::memory_order_acquire) || !g_logger) return;
        const auto ordinal = g_postExitTraceWriterCount.fetch_add(1, std::memory_order_acq_rel) + 1;
        const auto startNs = g_postExitTraceStartNs.load(std::memory_order_acquire);
        const auto nowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        const auto elapsedUs = startNs > 0 ? (nowNs - startNs) / 1000 : -1;
        DialoguePhase dialoguePhase = DialoguePhase::Inactive;
        {
            std::lock_guard dialogueLock(g_dialogueMutex);
            dialoguePhase = g_dialoguePhase;
        }
        Log("Post-EXIT FOV trace: seq=", g_postExitTraceSequence.load(std::memory_order_acquire),
            " ordinal=", ordinal, " elapsedUs=", elapsedUs,
            " source=0x", std::hex, source, " output=0x", output, std::dec,
            " writerInputXmm0=", writerInputXmm0,
            " cameraWorldFov(+0x230)=", cameraWorldFov,
            " cameraFirstPersonFov(+0x234)=", cameraFirstPersonFov,
            " aspect=", aspect, " flags=0x", std::hex, static_cast<int>(flags),
            " selector=0x", static_cast<int>(selector), std::dec,
            " outputFOV(before)=", outputFov, " outputAspect(before)=", outputAspect,
            " coordinator=", CoordinatorStateName(g_coordinator.load(std::memory_order_acquire)),
            " replayState=", ReplayStateName(g_state.load(std::memory_order_acquire)),
            " dialoguePhase=", DialoguePhaseName(dialoguePhase), ".");
        if (ordinal >= 32) g_postExitTraceArmed.store(false, std::memory_order_release);
    }

    void ObservePostExitRawWriterEntry(SafetyHookContext& context)
    {
        if (!g_postExitTraceArmed.load(std::memory_order_acquire)) return;
        const auto source = static_cast<std::uintptr_t>(context.rsi);
        const auto output = static_cast<std::uintptr_t>(context.rbx);
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        float cameraWorldFov = 0.0f;
        float cameraFirstPersonFov = 0.0f;
        float outputFov = 0.0f;
        float outputAspect = 0.0f;
        std::uint8_t selector = 0;
        if (!SafeRead(source + kAspectOffset, aspect) ||
            !SafeRead(source + kFlagsOffset, flags) ||
            !SafeRead(source + 0x230, cameraWorldFov) ||
            !SafeRead(source + 0x234, cameraFirstPersonFov) ||
            !SafeRead(source + 0x262, selector) ||
            !SafeRead(output + 0x30, outputFov) ||
            !SafeRead(output + 0x5C, outputAspect)) return;
        LogPostExitWriterObservation(source, output, context.xmm0.f32[0],
            cameraWorldFov, cameraFirstPersonFov, aspect, flags, selector,
            outputFov, outputAspect);
#ifdef POST_EXIT_INTERPOLATED_FOV_PRODUCER_TRACE
        std::uintptr_t returnAddress = 0;
        SafeRead(static_cast<std::uintptr_t>(context.rsp), returnAddress);
        Log("Post-EXIT interpolated FOV producer marker: seq=",
            g_postExitTraceSequence.load(std::memory_order_acquire),
            " elapsedUs=", [&]() {
                const auto startNs = g_postExitTraceStartNs.load(std::memory_order_acquire);
                const auto nowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count();
                return startNs > 0 ? (nowNs - startNs) / 1000 : -1;
            }(),
            " writerRIP=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(g_fovWriteAddress),
            " return=0x", returnAddress, " source=0x", source, std::dec,
            " writerInputXmm0=", context.xmm0.f32[0],
            " cameraWorldFov=", cameraWorldFov,
            " threadId=", GetCurrentThreadId(), ".");
#endif
    }

#ifdef POST_EXIT_FOV_PRODUCER_STORE_TRACE
    void ObservePostExitProducerStore(SafetyHookContext& context)
    {
        if (!g_logger) return;
        const auto camera = static_cast<std::uintptr_t>(context.rsi);
        const auto destination = camera + 0x230;
        float previousFov = 0.0f;
        const bool readable = camera != 0 && SafeRead(destination, previousFov);
        const bool armed = g_postExitTraceArmed.load(std::memory_order_acquire);
        const auto coordinator = g_coordinator.load(std::memory_order_acquire);
        const auto startNs = g_postExitTraceStartNs.load(std::memory_order_acquire);
        const auto nowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        const auto elapsedUs = startNs > 0 ? (nowNs - startNs) / 1000 : -1;
        const auto hitOrdinal = g_postExitProducerStoreHitCount.fetch_add(
            1, std::memory_order_acq_rel) + 1;
        Log("Post-EXIT FOV producer store HIT: hitOrdinal=", hitOrdinal,
            " armed=", armed, " elapsedUs=", elapsedUs,
            " storeRIP=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(g_executable) + kPostExitProducerStoreRva,
            " camera=0x", camera, " destination=0x", destination, std::dec,
            " previousFov=", previousFov, " writtenFov=", context.xmm0.f32[0],
            " readable=", readable, " coordinator=", CoordinatorStateName(coordinator),
            " sequence=", g_postExitTraceSequence.load(std::memory_order_acquire),
            " threadId=", GetCurrentThreadId(), ".");
        if (hitOrdinal >= 256) g_postExitProducerStoreHitCount.store(0, std::memory_order_release);
    }

    bool InstallPostExitProducerStoreTrace(const std::string& gameHash)
    {
        if (_stricmp(gameHash.c_str(), kPostExitProducerStoreGameSha256) != 0) {
            Log("Post-EXIT FOV producer store trace refused: game identity mismatch.");
            return false;
        }
        auto* target = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<std::uintptr_t>(g_executable) + kPostExitProducerStoreRva);
        constexpr std::uint8_t expected[] = {
            0xF3, 0x0F, 0x11, 0x86, 0x30, 0x02, 0x00, 0x00
        };
        if (!IsExecutable(reinterpret_cast<std::uintptr_t>(target)) ||
            std::memcmp(target, expected, sizeof(expected)) != 0) {
            Log("Post-EXIT FOV producer store trace refused: 2.0.5 store bytes mismatch.");
            return false;
        }
        g_postExitProducerStoreHook = safetyhook::create_mid(target, ObservePostExitProducerStore);
        if (!g_postExitProducerStoreHook) {
            Log("Post-EXIT FOV producer store trace refused: hook creation failed.");
            return false;
        }
        Log("Post-EXIT FOV producer store trace installed: RVA=0x32B779D; observation only.");
        return true;
    }
#endif

#ifdef POST_EXIT_CAMERA_FOV_WRITE_OWNER_TRACE
    void ObservePostExitFovWriteOwner(SafetyHookContext& context)
    {
        if (!g_logger) return;

        const auto camera = static_cast<std::uintptr_t>(context.rax);
        const auto destination = camera + 0x230;
        float previousFov = 0.0f;
        const bool readable = camera != 0 && SafeRead(camera + 0x230, previousFov);
        const bool armed = g_postExitTraceArmed.load(std::memory_order_acquire);
        const auto coordinator = g_coordinator.load(std::memory_order_acquire);
        const auto startNs = g_postExitTraceStartNs.load(std::memory_order_acquire);
        const auto nowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        const auto elapsedUs = startNs > 0 ? (nowNs - startNs) / 1000 : -1;
        std::uintptr_t returnAddress = 0;
        SafeRead(static_cast<std::uintptr_t>(context.rsp), returnAddress);
        const auto hitOrdinal = g_postExitWriteOwnerHitCount.fetch_add(1, std::memory_order_acq_rel) + 1;
        Log("Post-EXIT FOV store candidate HIT: hitOrdinal=", hitOrdinal,
            " armed=", armed, " elapsedUs=", elapsedUs,
            " writeRIP=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(g_executable) + kPostExitWriteOwnerRva,
            " return=0x", returnAddress, " camera=0x", camera, std::dec,
            " destination=0x", std::hex, destination, std::dec,
            " previousFov=", previousFov,
            " writtenFov=", context.xmm0.f32[0],
            " readable=", readable,
            " coordinator=", CoordinatorStateName(coordinator),
            " sequence=", g_postExitTraceSequence.load(std::memory_order_acquire),
            " threadId=", GetCurrentThreadId(), ".");
        if (hitOrdinal >= 256) g_postExitWriteOwnerHitCount.store(0, std::memory_order_release);
    }

    bool InstallPostExitFovWriteOwnerTrace(const std::string& gameHash)
    {
        if (_stricmp(gameHash.c_str(), kPostExitWriteOwnerGameSha256) != 0) {
            Log("Post-EXIT FOV physical-write trace refused: game identity mismatch.");
            return false;
        }
        auto* target = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<std::uintptr_t>(g_executable) + kPostExitWriteOwnerRva);
        constexpr std::uint8_t expected[] = {
            0xF3, 0x0F, 0x11, 0x80, 0x30, 0x02, 0x00, 0x00
        };
        if (!IsExecutable(reinterpret_cast<std::uintptr_t>(target)) ||
            std::memcmp(target, expected, sizeof(expected)) != 0) {
            Log("Post-EXIT FOV physical-write trace refused: 2.0.5 store bytes mismatch.");
            return false;
        }
        g_postExitWriteOwnerHook = safetyhook::create_mid(target, ObservePostExitFovWriteOwner);
        if (!g_postExitWriteOwnerHook) {
            Log("Post-EXIT FOV physical-write trace refused: hook creation failed.");
            return false;
        }
        Log("Post-EXIT FOV physical-write trace installed: RVA=0x3DB2CE7; observation only.");
        return true;
    }
#endif

#ifdef POST_EXIT_FOV_STATE_CONSUMER_TRACE
    void ObservePostExitFovConsumer(SafetyHookContext& context)
    {
        const auto coordinator = g_coordinator.load(std::memory_order_acquire);
#ifdef CINEMATIC_FOV_TRANSITION_TRACE
        if (!g_logger || (coordinator != CoordinatorState::CinematicActive &&
            coordinator != CoordinatorState::CinematicExiting)) return;
#else
        if (!g_postExitTraceArmed.load(std::memory_order_acquire) ||
            coordinator != CoordinatorState::CinematicExiting || !g_logger) return;
#endif
        const auto state = static_cast<std::uintptr_t>(context.rcx);
        const auto callerAddress = [&]() {
            std::uintptr_t value{};
            SafeRead(static_cast<std::uintptr_t>(context.rsp), value);
            return value;
        }();
        float state50 = 0.0f;
        float state54 = 0.0f;
        float state58 = 0.0f;
        const bool reads = state != 0 &&
            SafeRead(state + 0x50, state50) && SafeRead(state + 0x54, state54) &&
            SafeRead(state + 0x58, state58);
        const auto startNs = g_postExitTraceStartNs.load(std::memory_order_acquire);
        const auto nowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        const auto elapsedUs = coordinator == CoordinatorState::CinematicExiting && startNs > 0
            ? (nowNs - startNs) / 1000 : -1;
        Log("Cinematic FOV consumer: phase=", CoordinatorStateName(coordinator),
            " seq=", g_postExitTraceSequence.load(std::memory_order_acquire),
            " elapsedUs=", elapsedUs, " incomingFov=", context.xmm1.f32[0],
            " state=0x", std::hex, state, " caller=0x", callerAddress, std::dec,
            " threadId=", GetCurrentThreadId(), " stateRead=", reads,
            " state+0x50=", state50, " state+0x54(before)=", state54,
            " state+0x58=", state58, ".");
    }

    bool InstallPostExitFovConsumerTrace(const std::string& gameHash)
    {
        if (_stricmp(gameHash.c_str(), kPostExitConsumerGameSha256) != 0) {
            Log("Post-EXIT FOV consumer trace refused: game identity mismatch.");
            return false;
        }
        auto* target = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<std::uintptr_t>(g_executable) + kPostExitConsumerRva);
        constexpr std::uint8_t expected[] = {
            0x56, 0x57, 0x48, 0x83, 0xEC, 0x28, 0x48, 0x89, 0xCE,
            0xF3, 0x0F, 0x11
        };
        if (!IsExecutable(reinterpret_cast<std::uintptr_t>(target)) ||
            std::memcmp(target, expected, sizeof(expected)) != 0) {
            Log("Post-EXIT FOV consumer trace refused: 2.0.5 prologue mismatch.");
            return false;
        }
        g_postExitConsumerHook = safetyhook::create_mid(target, ObservePostExitFovConsumer);
        if (!g_postExitConsumerHook) {
            Log("Post-EXIT FOV consumer trace refused: hook creation failed.");
            return false;
        }
        Log("Post-EXIT FOV consumer trace installed: RVA=0x318DCD4; observation only.");
        return true;
    }
#endif

#ifdef FOV_SETTINGS_TRACE_DIAGNOSTIC
    void LogGameplayFovChange(std::uintptr_t source, float fov)
    {
        const auto coordinator = g_coordinator.load(std::memory_order_acquire);
        const auto replayState = g_state.load(std::memory_order_acquire);
        if (source != g_lastLoggedGameplayFovSource ||
            !std::isfinite(g_lastLoggedGameplayFov) ||
            std::fabs(fov - g_lastLoggedGameplayFov) > 0.01f) {
            Log("Gameplay FOV trace: source=0x", std::hex, source, std::dec,
                " writerInputXmm0=", fov,
                " coordinator=", static_cast<std::uint32_t>(coordinator),
                " replayState=", static_cast<std::uint32_t>(replayState), ".");
            g_lastLoggedGameplayFovSource = source;
            g_lastLoggedGameplayFov = fov;
        }
    }
#endif

    bool ComputeSha256(const std::filesystem::path& path, std::string& result)
    {
        const auto file = CreateFileW(path.c_str(), GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file == INVALID_HANDLE_VALUE) return false;

        BCRYPT_ALG_HANDLE algorithm{};
        BCRYPT_HASH_HANDLE hash{};
        std::vector<std::uint8_t> hashObject;
        std::vector<std::uint8_t> buffer(1024 * 1024);
        std::array<std::uint8_t, 32> digest{};
        ULONG objectLength = 0;
        ULONG propertyLength = 0;
        bool success = false;

        do {
            if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) != 0 ||
                BCryptGetProperty(algorithm, BCRYPT_OBJECT_LENGTH,
                    reinterpret_cast<PUCHAR>(&objectLength), sizeof(objectLength), &propertyLength, 0) != 0 ||
                objectLength == 0) break;
            hashObject.resize(objectLength);
            if (BCryptCreateHash(algorithm, &hash, hashObject.data(), objectLength,
                nullptr, 0, 0) != 0) break;

            for (;;) {
                DWORD count = 0;
                if (!ReadFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &count, nullptr)) break;
                if (count == 0) {
                    if (BCryptFinishHash(hash, digest.data(), static_cast<ULONG>(digest.size()), 0) != 0) break;
                    static constexpr char digits[] = "0123456789ABCDEF";
                    result.clear();
                    result.reserve(digest.size() * 2);
                    for (const auto byte : digest) {
                        result.push_back(digits[byte >> 4]);
                        result.push_back(digits[byte & 0x0F]);
                    }
                    success = true;
                    break;
                }
                if (BCryptHashData(hash, buffer.data(), count, 0) != 0) break;
            }
        } while (false);

        if (hash) BCryptDestroyHash(hash);
        if (algorithm) BCryptCloseAlgorithmProvider(algorithm, 0);
        CloseHandle(file);
        return success;
    }

    std::string Trim(std::string value)
    {
        const auto notSpace = [](unsigned char character) { return !std::isspace(character); };
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
        value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
        return value;
    }

    bool ParseBool(std::string value, bool& result)
    {
        value = Trim(std::move(value));
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
        if (value == "1" || value == "true" || value == "yes" || value == "on") {
            result = true;
            return true;
        }
        if (value == "0" || value == "false" || value == "no" || value == "off") {
            result = false;
            return true;
        }
        return false;
    }

    bool ParseHotkey(std::string value, int& result)
    {
        value = Trim(std::move(value));
        if (value.size() == 1) {
            const char key = static_cast<char>(std::toupper(static_cast<unsigned char>(value.front())));
            if (key >= 'A' && key <= 'Z') { result = key; return true; }
            if (key >= '0' && key <= '9') { result = key; return true; }
            return false;
        }
        if (value.size() < 2 || (value.front() != 'F' && value.front() != 'f')) return false;
        int number = 0;
        for (std::size_t index = 1; index < value.size(); ++index) {
            if (value[index] < '0' || value[index] > '9') return false;
            number = number * 10 + (value[index] - '0');
        }
        if (number < 1 || number > 12) return false;
        result = VK_F1 + number - 1;
        return true;
    }

    const char* HotkeyName(int key)
    {
        static const char* names[] = {
            "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
            "F9", "F10", "F11", "F12"
        };
        if (key >= VK_F1 && key <= VK_F12) return names[key - VK_F1];
        static thread_local char singleKey[2];
        if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z')) {
            singleKey[0] = static_cast<char>(key);
            singleKey[1] = '\0';
            return singleKey;
        }
        return "unknown";
    }

    bool ParseCinematicAspectPolicy(std::string value, CinematicAspectPolicy& result)
    {
        value = Trim(std::move(value));
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
        if (value == "auto" || value == "default" || value == "fix") result = CinematicAspectPolicy::Auto;
        else if (value == "native") result = CinematicAspectPolicy::Native;
        else if (value == "16:9" || value == "16x9") result = CinematicAspectPolicy::Forced16x9;
        else if (value == "21:9" || value == "21x9") result = CinematicAspectPolicy::Forced21x9;
        else if (value == "32:9" || value == "32x9") result = CinematicAspectPolicy::Forced32x9;
        else return false;
        return true;
    }

    const char* CinematicAspectPolicyName(CinematicAspectPolicy policy)
    {
        switch (policy) {
        case CinematicAspectPolicy::Auto: return "Auto";
        case CinematicAspectPolicy::Native: return "Native";
        case CinematicAspectPolicy::Forced16x9: return "16:9";
        case CinematicAspectPolicy::Forced21x9: return "21:9";
        case CinematicAspectPolicy::Forced32x9: return "32:9";
        }
        return "Auto";
    }

    bool ParseDialogueZoomPolicy(std::string value, DialogueZoomPolicy& result)
    {
        value = Trim(std::move(value));
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
        if (value == "native") result = DialogueZoomPolicy::Native;
        else if (value == "adaptive") result = DialogueZoomPolicy::Adaptive;
        else if (value == "reduced") result = DialogueZoomPolicy::Reduced;
        else if (value == "disabled") result = DialogueZoomPolicy::Disabled;
        else return false;
        return true;
    }

    const char* DialogueZoomPolicyName(DialogueZoomPolicy policy)
    {
        switch (policy) {
        case DialogueZoomPolicy::Native: return "Native";
        case DialogueZoomPolicy::Adaptive: return "Adaptive";
        case DialogueZoomPolicy::Reduced: return "Reduced";
        case DialogueZoomPolicy::Disabled: return "Disabled";
        }
        return "Native";
    }

    DialogueZoomPolicy NextDialogueZoomPolicy(DialogueZoomPolicy policy)
    {
        switch (policy) {
        case DialogueZoomPolicy::Native: return DialogueZoomPolicy::Adaptive;
        case DialogueZoomPolicy::Adaptive: return DialogueZoomPolicy::Reduced;
        case DialogueZoomPolicy::Reduced: return DialogueZoomPolicy::Disabled;
        case DialogueZoomPolicy::Disabled: return DialogueZoomPolicy::Native;
        }
        return DialogueZoomPolicy::Native;
    }

    CinematicAspectPolicy NextCinematicAspectPolicy(CinematicAspectPolicy policy)
    {
        switch (policy) {
        case CinematicAspectPolicy::Auto: return CinematicAspectPolicy::Native;
        case CinematicAspectPolicy::Native: return CinematicAspectPolicy::Forced16x9;
        case CinematicAspectPolicy::Forced16x9: return CinematicAspectPolicy::Forced21x9;
        case CinematicAspectPolicy::Forced21x9: return CinematicAspectPolicy::Forced32x9;
        case CinematicAspectPolicy::Forced32x9: return CinematicAspectPolicy::Auto;
        }
        return CinematicAspectPolicy::Auto;
    }

    bool CinematicAspectOverrideEnabled()
    {
        return g_runtimeCinematicPolicy.load(std::memory_order_acquire) != CinematicAspectPolicy::Native;
    }

    bool SynchronizeManagedConfigTemplate(const std::filesystem::path& path)
    {
        std::ifstream input(path);
        if (!input) return false;

        std::vector<std::string> source;
        std::string line;
        while (std::getline(input, line)) source.push_back(std::move(line));

        constexpr const char* kCinematicComments[] = {
            "; Controls cinematic framing on ultrawide displays.",
            "; Auto   - use the detected display aspect ratio.",
            "; Native - keep the game's original cinematic behavior.",
            "; 16:9   - force the native 16:9 cinematic frame.",
            "; 21:9   - force a 21:9 cinematic frame.",
            "; 32:9   - force a 32:9 cinematic frame.",
        };
        constexpr const char* kDialogueComments[] = {
            "; Controls the native dialogue camera zoom.",
            "; Native   - use the game's original dialogue zoom, currently targeting 70°.",
            "; Adaptive - preserve the native optical zoom strength relative to the current gameplay FOV.",
            "; Reduced  - apply half of the Adaptive optical zoom strength.",
            ";            Example: 110° gameplay FOV -> Adaptive ≈90°, Reduced ≈100°.",
            "; Disabled - keep the current gameplay FOV during dialogue.",
        };
        constexpr const char* kHotkeyComments[] = {
            "; Enables or disables all runtime hotkeys.",
            "; Supported keys: F1-F12, 0-9 and A-Z.",
            "; Key used to cycle the cinematic mode for the next cinematic.",
            "; Auto -> Native -> 16:9 -> 21:9 -> 32:9 -> Auto.",
            "; Does not affect a cinematic that is already playing.",
            "; Key used to cycle the dialogue zoom mode for the next dialogue.",
            "; Native -> Adaptive -> Reduced -> Disabled -> Native.",
            "; Does not affect a dialogue that is already in progress.",
        };

        std::vector<std::string> output;
        output.reserve(source.size() + 16);
        std::string section;
        std::array<bool, 4> hasManagedComments{};
        std::array<bool, 4> insertedManagedComments{};
        bool legacyHotkeyComments = false;
        bool hasHotkeyRangeComment = false;
        for (const auto& original : source) {
            const auto trimmed = Trim(original);
            if (trimmed.size() >= 2 && trimmed.front() == '[' && trimmed.back() == ']') {
                section = Trim(trimmed.substr(1, trimmed.size() - 2));
                continue;
            }
            const auto mark = [&](std::size_t index, const auto& comments) {
                for (const auto* comment : comments)
                    if (trimmed == comment) hasManagedComments[index] = true;
            };
            if (section == "Gameplay" && (trimmed == "; Correct gameplay aspect behavior on ultrawide displays." ||
                trimmed == "; Enables ultrawide aspect-ratio correction during gameplay."))
                hasManagedComments[0] = true;
            else if (section == "Cinematics") {
                mark(1, kCinematicComments);
                if (trimmed == "; Auto, Native, 16:9, 21:9, 32:9") hasManagedComments[1] = true;
            }
            else if (section == "Dialogue") {
                mark(2, kDialogueComments);
                if (trimmed == "; Native, Reduced, Disabled") hasManagedComments[2] = true;
            }
            else if (section == "Hotkeys") mark(3, kHotkeyComments);
            if (section == "Hotkeys" && trimmed == "; Supported keys: F1-F12, 0-9 and A-Z.")
                hasHotkeyRangeComment = true;
            if (section == "Hotkeys" && (trimmed == "; F9 cycles cinematic mode for the next cinematic:" ||
                trimmed == "; F10 cycles dialogue mode for the next dialogue:"))
                legacyHotkeyComments = true;
        }

        section.clear();
        bool foundHotkeys = false;
        bool foundHotkeyEnabled = false;
        bool foundCinematicCycle = false;
        bool foundDialogueCycle = false;
        bool changed = false;
        for (const auto& original : source) {
            const auto trimmed = Trim(original);
            if (trimmed == "; STALKER 2 Ultrawide Fix v0.4.0" ||
                trimmed == "; STALKER 2 Ultrawide Fix v0.5.0" ||
                trimmed == "; STALKER 2 Ultrawide and Camera Tweaks v0.5.0" ||
                trimmed == "; STALKER 2 Ultrawide and Camera Tweaks v0.5.1" ||
                trimmed == "; STALKER 2 Ultrawide Fix v0.5.2") {
                output.emplace_back("; STALKER 2 Ultrawide and Camera Tweaks v0.5.2");
                changed = true;
                continue;
            }
            if (trimmed.size() >= 2 && trimmed.front() == '[' && trimmed.back() == ']') {
                section = Trim(trimmed.substr(1, trimmed.size() - 2));
                if (section == "Hotkeys") foundHotkeys = true;
                if (section == "Cinematics" || section == "Dialogue" || section == "Hotkeys") {
                    while (!output.empty() && output.back().empty()) output.pop_back();
                    output.emplace_back("");
                    output.emplace_back("");
                }
            }

            const auto separator = trimmed.find('=');
            const bool firstManagedKey = separator != std::string::npos &&
                (section == "Gameplay" || section == "Cinematics" ||
                 section == "Dialogue" || section == "Hotkeys");
            if (firstManagedKey) {
                const auto key = Trim(trimmed.substr(0, separator));
                if (section == "Hotkeys") {
                    if (key == "Enabled" && !hasManagedComments[3] && !insertedManagedComments[3]) {
                        output.emplace_back(kHotkeyComments[0]);
                        output.emplace_back(kHotkeyComments[1]);
                        insertedManagedComments[3] = true;
                        changed = true;
                    } else if (key == "Enabled" && !hasHotkeyRangeComment && hasManagedComments[3]) {
                        output.emplace_back(kHotkeyComments[1]);
                        hasHotkeyRangeComment = true;
                        changed = true;
                    } else if (key == "CinematicCycle" && legacyHotkeyComments) {
                        if (!output.empty() && !output.back().empty()) output.emplace_back("");
                        output.emplace_back(kHotkeyComments[2]);
                        output.emplace_back(kHotkeyComments[3]);
                        output.emplace_back(kHotkeyComments[4]);
                        changed = true;
                    } else if (key == "DialogueCycle" && legacyHotkeyComments) {
                        if (!output.empty() && !output.back().empty()) output.emplace_back("");
                        output.emplace_back(kHotkeyComments[5]);
                        output.emplace_back(kHotkeyComments[6]);
                        output.emplace_back(kHotkeyComments[7]);
                        changed = true;
                    }
                } else {
                    std::size_t sectionIndex = section == "Gameplay" ? 0 :
                        section == "Cinematics" ? 1 : 2;
                    if (!hasManagedComments[sectionIndex] && !insertedManagedComments[sectionIndex]) {
                        if (section == "Gameplay")
                        output.emplace_back("; Enables ultrawide aspect-ratio correction during gameplay.");
                        else if (section == "Cinematics")
                            for (const auto* comment : kCinematicComments) output.emplace_back(comment);
                        else
                            for (const auto* comment : kDialogueComments) output.emplace_back(comment);
                        insertedManagedComments[sectionIndex] = true;
                        changed = true;
                    }
                }
            }

            if (section == "Hotkeys" && legacyHotkeyComments && (trimmed == "; F9 cycles cinematic mode for the next cinematic:" ||
                trimmed == "; F10 cycles dialogue mode for the next dialogue:" ||
                trimmed == "; Auto -> Native -> 16:9 -> 21:9 -> 32:9 -> Auto." ||
                trimmed == "; It does not change a cinematic that is already playing." ||
                trimmed == "; Native -> Reduced -> Disabled -> Native." ||
                trimmed == "; It does not change a dialogue that is already in progress.")) {
                changed = true;
                continue;
            }

            if (section == "Hotkeys" && (trimmed == "; Key used to cycle the cinematic mode for the next cinematic." ||
                trimmed == "; Key used to cycle the dialogue zoom mode for the next dialogue.")) {
                if (!output.empty() && !output.back().empty()) {
                    output.emplace_back("");
                    changed = true;
                }
            }

            if (section == "Gameplay" && trimmed == "; Correct gameplay aspect behavior on ultrawide displays.") {
                output.emplace_back("; Enables ultrawide aspect-ratio correction during gameplay.");
                changed = true;
                continue;
            }
            if (section == "Cinematics" && trimmed == "; Auto, Native, 16:9, 21:9, 32:9") {
                for (const auto* comment : kCinematicComments) output.emplace_back(comment);
                changed = true;
                continue;
            }
            if (section == "Dialogue" && trimmed == "; Native, Reduced, Disabled") {
                for (const auto* comment : kDialogueComments) output.emplace_back(comment);
                changed = true;
                continue;
            }
            if (section == "Hotkeys") {
                const auto separator = trimmed.find('=');
                if (separator != std::string::npos) {
                    const auto key = Trim(trimmed.substr(0, separator));
                    if (key == "Enabled") foundHotkeyEnabled = true;
                    if (key == "CinematicCycle") foundCinematicCycle = true;
                    if (key == "DialogueCycle") foundDialogueCycle = true;
                }
            }
            output.push_back(original);
        }

        if (!foundHotkeys) {
            while (!output.empty() && output.back().empty()) output.pop_back();
            output.emplace_back("");
            output.emplace_back("");
            output.emplace_back("[Hotkeys]");
            for (const auto* comment : kHotkeyComments) output.emplace_back(comment);
            output.emplace_back("Enabled=false");
            output.emplace_back("CinematicCycle=F9");
            output.emplace_back("DialogueCycle=F10");
            changed = true;
        } else {
            if (!foundHotkeyEnabled) { output.emplace_back("Enabled=false"); changed = true; }
            if (!foundCinematicCycle) { output.emplace_back("CinematicCycle=F9"); changed = true; }
            if (!foundDialogueCycle) { output.emplace_back("DialogueCycle=F10"); changed = true; }
        }

        if (!changed && output == source) return false;
        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if (!file) return false;
        for (std::size_t index = 0; index < output.size(); ++index) {
            file << output[index];
            if (index + 1 < output.size()) file << '\n';
        }
        return static_cast<bool>(file);
    }

    bool LoadFeatureConfig(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path)) {
            std::ofstream created(path, std::ios::out | std::ios::trunc);
            if (!created) return false;
            created << "; STALKER 2 Ultrawide and Camera Tweaks v0.6.0\n"
                << "; Author: Elhait\n"
                << "; GitHub: https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4\n"
                << "; Nexus Mods: https://www.nexusmods.com/stalker2heartofchornobyl/mods/2416\n"
                << "; Configuration changes apply after restarting the game. F9/F10 apply to the next applicable state.\n"
                << "\n[Gameplay]\n"
                << "; Enables ultrawide aspect-ratio correction during gameplay.\n"
                << "; Use true to enable the feature or false to disable it.\n"
                << "Enabled=true\n"
                << "\n\n\n[Cinematics]\n"
                << "; Controls cinematic framing on ultrawide displays.\n"
                << "; Auto   - use the detected display aspect ratio.\n"
                << "; Native - keep the game's original cinematic behavior.\n"
                << "; 16:9   - force the native 16:9 cinematic frame.\n"
                << "; 21:9   - force a 21:9 cinematic frame.\n"
                << "; 32:9   - force a 32:9 cinematic frame.\n"
                << "AspectRatio=Auto\n"
                << "\n\n\n[Dialogue]\n"
                << "; Controls the native dialogue camera zoom.\n"
                << "; Native   - use the game's original dialogue zoom, currently targeting 70°.\n"
                << "; Adaptive - preserve the native optical zoom strength relative to the current gameplay FOV.\n"
                << "; Reduced  - apply half of the Adaptive optical zoom strength.\n"
                << ";            Example: 110° gameplay FOV -> Adaptive ≈90°, Reduced ≈100°.\n"
                << "; Disabled - keep the current gameplay FOV during dialogue.\n"
                << "Zoom=Reduced\n"
                << "\n\n\n[Hotkeys]\n"
                << "; Enables or disables all runtime hotkeys.\n"
                << "; Use true to enable all runtime hotkeys or false to disable them.\n"
                << "Enabled=false\n"
                << "\n"
                << "; Key used to cycle the cinematic mode for the next cinematic.\n"
                << "; Auto -> Native -> 16:9 -> 21:9 -> 32:9 -> Auto.\n"
                << "; Does not affect a cinematic that is already playing.\n"
                << "; Supported keys: F1-F12, 0-9 and A-Z.\n"
                << "CinematicCycle=F9\n"
                << "; Key used to cycle the dialogue zoom mode for the next dialogue.\n"
                << "; Native -> Adaptive -> Reduced -> Disabled -> Native.\n"
                << "; Does not affect a dialogue that is already in progress.\n"
                << "; Supported keys: F1-F12, 0-9 and A-Z.\n"
                << "DialogueCycle=F10\n";
            return static_cast<bool>(created);
        }

        if (SynchronizeManagedConfigTemplate(path))
            Log("Config template synchronized: updated managed descriptions and hotkey settings.");

        std::ifstream input(path);
        if (!input) return false;
        std::string section;
        std::string line;
        while (std::getline(input, line)) {
            line = Trim(line);
            if (line.empty() || line.front() == ';' || line.front() == '#') continue;
            if (line.front() == '[' && line.back() == ']') {
                section = Trim(line.substr(1, line.size() - 2));
                continue;
            }
            const auto separator = line.find('=');
            if (separator == std::string::npos) continue;
            const auto key = Trim(line.substr(0, separator));
            if (section == "Cinematics" && key == "AspectRatio") {
                CinematicAspectPolicy policy{};
                if (ParseCinematicAspectPolicy(line.substr(separator + 1), policy)) {
                    g_config.cinematicAspectPolicy = policy;
                    g_config.cinematicAspectPolicyExplicit = true;
                }
                continue;
            }
            if (section == "Dialogue" && key == "Zoom") {
                DialogueZoomPolicy policy{};
                if (ParseDialogueZoomPolicy(line.substr(separator + 1), policy))
                    g_config.dialogueZoomPolicy = policy;
                continue;
            }
            if (section == "Hotkeys" && key == "Enabled") {
                bool enabled = true;
                if (ParseBool(line.substr(separator + 1), enabled)) g_config.hotkeysEnabled = enabled;
                continue;
            }
            if (section == "Hotkeys" && key == "CinematicCycle") {
                int hotkey = VK_F9;
                if (ParseHotkey(line.substr(separator + 1), hotkey)) g_config.cinematicCycleKey = hotkey;
                continue;
            }
            if (section == "Hotkeys" && key == "DialogueCycle") {
                int hotkey = VK_F10;
                if (ParseHotkey(line.substr(separator + 1), hotkey)) g_config.dialogueCycleKey = hotkey;
                continue;
            }
            bool value = true;
            if (!ParseBool(line.substr(separator + 1), value)) continue;
            if (section == "Gameplay" && key == "Enabled") g_config.gameplayEnabled = value;
            else if (section == "Cinematics" && key == "AspectFix" && !g_config.cinematicAspectPolicyExplicit)
                g_config.cinematicAspectPolicy = value ? CinematicAspectPolicy::Auto : CinematicAspectPolicy::Native;
            else if (section == "Features" && key == "GameplayAspectFix") g_config.gameplayEnabled = value;
            else if (section == "Features" && key == "CinematicAspectFix" && !g_config.cinematicAspectPolicyExplicit)
                g_config.cinematicAspectPolicy = value ? CinematicAspectPolicy::Auto : CinematicAspectPolicy::Native;
        }
        return true;
    }

    bool PersistConfigValue(const std::filesystem::path& path, const char* targetSection,
        const char* targetKey, const std::string& value)
    {
        std::ifstream input(path);
        if (!input) return false;
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(input, line)) lines.push_back(std::move(line));

        bool inSection = false;
        bool replaced = false;
        for (auto& current : lines) {
            const auto trimmed = Trim(current);
            if (trimmed.size() >= 2 && trimmed.front() == '[' && trimmed.back() == ']')
                inSection = Trim(trimmed.substr(1, trimmed.size() - 2)) == targetSection;
            if (!inSection) continue;
            const auto separator = trimmed.find('=');
            if (separator != std::string::npos && Trim(trimmed.substr(0, separator)) == targetKey) {
                current = std::string(targetKey) + "=" + value;
                replaced = true;
                break;
            }
        }
        if (!replaced) {
            lines.push_back("");
            lines.push_back(std::string("[") + targetSection + "]");
            lines.push_back(std::string(targetKey) + "=" + value);
        }

        const auto temporary = path.wstring() + L".tmp";
        {
            std::ofstream output(temporary, std::ios::out | std::ios::trunc);
            if (!output) return false;
            for (std::size_t index = 0; index < lines.size(); ++index) {
                output << lines[index];
                if (index + 1 < lines.size()) output << '\n';
            }
            if (!output) return false;
        }
        if (MoveFileExW(temporary.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
            return true;

        const auto replaceError = GetLastError();
        Log("Atomic INI replacement unavailable: win32Error=", replaceError,
            "; using direct-write fallback.");
        std::ofstream output(path, std::ios::out | std::ios::trunc);
        if (output) {
            for (std::size_t index = 0; index < lines.size(); ++index) {
                output << lines[index];
                if (index + 1 < lines.size()) output << '\n';
            }
            if (output) {
                DeleteFileW(temporary.c_str());
                return true;
            }
        }
        const auto directError = GetLastError();
        Log("Config persistence direct write failed: replaceError=", replaceError,
            " directError=", directError, ".");
        DeleteFileW(temporary.c_str());
        return false;
    }

    bool IsWritable(std::uintptr_t address, std::size_t size)
    {
        MEMORY_BASIC_INFORMATION info{};
        if (!address || !VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
        const auto regionEnd = reinterpret_cast<std::uintptr_t>(info.BaseAddress) + info.RegionSize;
        const DWORD writable = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        return address <= regionEnd && size <= regionEnd - address && (info.Protect & writable);
    }

    template <typename T>
    bool SafeRead(std::uintptr_t address, T& value)
    {
        MEMORY_BASIC_INFORMATION info{};
        if (!address || !VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
        const auto regionEnd = reinterpret_cast<std::uintptr_t>(info.BaseAddress) + info.RegionSize;
        if (address > regionEnd || sizeof(value) > regionEnd - address) return false;
        std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(value));
        return true;
    }

    float ReadRuntimeAspect(std::uintptr_t object)
    {
        float aspect = 0.0f;
        if (object && SafeRead(object + kAspectOffset, aspect) && IsValidAspect(aspect))
            return aspect;
        const float observed = g_lastObservedAspect.load(std::memory_order_acquire);
        return IsValidAspect(observed) ? observed : kNativeAspect;
    }

    BOOL CALLBACK FindCurrentProcessWindowForAspect(HWND window, LPARAM parameter)
    {
        DWORD processId = 0;
        GetWindowThreadProcessId(window, &processId);
        if (processId == GetCurrentProcessId() && IsWindowVisible(window) &&
            GetWindow(window, GW_OWNER) == nullptr) {
            *reinterpret_cast<HWND*>(parameter) = window;
            return FALSE;
        }
        return TRUE;
    }

    float ReadClientViewportAspect()
    {
        HWND window = GetForegroundWindow();
        DWORD processId = 0;
        if (!window || (GetWindowThreadProcessId(window, &processId),
            processId != GetCurrentProcessId())) {
            window = nullptr;
            EnumWindows(FindCurrentProcessWindowForAspect, reinterpret_cast<LPARAM>(&window));
        }

        RECT client{};
        if (window && GetClientRect(window, &client)) {
            const auto width = client.right - client.left;
            const auto height = client.bottom - client.top;
            if (width > 0 && height > 0) {
                return static_cast<float>(width) / static_cast<float>(height);
            }
        }

        DEVMODE display{ .dmSize = sizeof(DEVMODE) };
        if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &display) && display.dmPelsHeight != 0) {
            return static_cast<float>(display.dmPelsWidth) / static_cast<float>(display.dmPelsHeight);
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    float ResolveAutoAspect()
    {
        const float aspect = ReadClientViewportAspect();
        if (IsValidAspect(aspect)) {
            Log("Auto cinematic aspect resolved from client/display viewport: aspect=", aspect, ".");
            return aspect;
        }
        Log("Auto cinematic aspect resolution unavailable; native aspect fallback=", kNativeAspect, ".");
        return kNativeAspect;
    }

    float ResolveCinematicAspect(std::uintptr_t object)
    {
        switch (g_runtimeCinematicPolicy.load(std::memory_order_acquire)) {
        case CinematicAspectPolicy::Forced16x9: return kNativeAspect;
        case CinematicAspectPolicy::Forced21x9: return kCinemaAspect;
        case CinematicAspectPolicy::Forced32x9: return kWideAspect;
        case CinematicAspectPolicy::Auto: return ResolveAutoAspect();
        case CinematicAspectPolicy::Native: return kNativeAspect;
        }
        return kNativeAspect;
    }

    bool WriteAspectAndFlags(std::uintptr_t source, float aspect, std::uint8_t flags)
    {
        if (!IsWritable(source + kAspectOffset, sizeof(aspect)) || !IsWritable(source + kFlagsOffset, sizeof(flags))) return false;
        std::memcpy(reinterpret_cast<void*>(source + kAspectOffset), &aspect, sizeof(aspect));
        std::memcpy(reinterpret_cast<void*>(source + kFlagsOffset), &flags, sizeof(flags));
        return true;
    }

#if defined(GAMEPLAY_FIX_ATOMICITY_TEST) || \
    defined(POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST)
    bool ApplyGameplayAspectFixAtomic(std::uintptr_t source, const char* phase, float fov,
        float previousAspect, std::uint8_t previousFlags)
    {
        if (!WriteAspectAndFlags(source, kNativeAspect, 0x4)) {
            Log("AtomicReplayRefused: phase=", phase,
                " final aspect/flags fields were not writable.");
            return false;
        }
        g_state.store(ReplayState::Complete, std::memory_order_release);
        g_lastAutoRestoreSource.store(source, std::memory_order_release);
        Log("AtomicReplayApplied: phase=", phase, " source=0x", std::hex, source,
            std::dec, " fov=", fov, " aspect=", kNativeAspect,
            " flags=0x4 previousAspect=", previousAspect, " previousFlags=0x",
            std::hex, static_cast<unsigned>(previousFlags), std::dec, ".");
        return true;
    }
#endif

    void ApplyCinematicAspectStore(SafetyHookContext& context)
    {
        const auto targetObject = static_cast<std::uintptr_t>(context.rax);
        const auto policy = g_runtimeCinematicPolicy.load(std::memory_order_acquire);
        const float observedAspect = ReadRuntimeAspect(targetObject);
        // For Auto, the native store may already have written 16:9 into the
        // object by the time this boundary is observed. Use the cached runtime
        // camera aspect, which is also the source used by the FOV boundary.
        const float resolvedAspect = ResolveCinematicAspect(policy == CinematicAspectPolicy::Auto ? 0 : targetObject);
        const float aspect = IsValidAspect(resolvedAspect) ? resolvedAspect : kNativeAspect;
        const bool writable = targetObject &&
            targetObject <= (std::numeric_limits<std::uintptr_t>::max)() - kAspectOffset &&
            IsWritable(targetObject + kAspectOffset, sizeof(aspect));
        if (writable) {
            std::memcpy(reinterpret_cast<void*>(targetObject + kAspectOffset), &aspect, sizeof(aspect));
            Log("Cinematic aspect store: object=0x", std::hex, targetObject, std::dec,
                " aspect=", aspect, " policy=", CinematicAspectPolicyName(policy),
                " source=", policy == CinematicAspectPolicy::Auto
                    ? (IsValidAspect(observedAspect) ? "runtime-camera" : "native-fallback")
                    : "configured", ".");
        } else {
            Log("Cinematic aspect store refused: target object was not writable; native store skipped.");
        }
        // The hook replaces C7 80 [disp32] [imm32]. The original store has no
        // control-flow side effects, so resuming after its ten-byte encoding
        // preserves the native lifecycle and following instructions.
        context.rip += kCinematicStoreInstructionLength;
    }

    bool LogCameraModeChange(std::uintptr_t source, std::uintptr_t output, float writerInputXmm0,
        float aspect, std::uint8_t flags)
    {
        float cameraWorldFov = 0.0f;
        float cameraFirstPersonFov = 0.0f;
        float outputFov = 0.0f;
        float outputAspect = 0.0f;
        float rawSourceAspect = 0.0f;
        std::uint8_t rawSourceFlags = 0;
        std::uint8_t selector = 0;
        SafeRead(source + 0x230, cameraWorldFov);
        SafeRead(source + 0x234, cameraFirstPersonFov);
        SafeRead(source + kAspectOffset, rawSourceAspect);
        SafeRead(source + kFlagsOffset, rawSourceFlags);
        SafeRead(source + 0x262, selector);
        SafeRead(output + 0x30, outputFov);
        SafeRead(output + 0x5C, outputAspect);
        LogGameplayFovSourceChange(source, writerInputXmm0, cameraWorldFov,
            cameraFirstPersonFov, aspect, flags);

#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        std::uint64_t outputField40 = 0;
        std::uint64_t outputField58 = 0;
        std::uint64_t outputField68 = 0;
        if (!SafeRead(output + 0x40, outputField40) || !SafeRead(output + 0x58, outputField58) ||
            !SafeRead(output + 0x68, outputField68))
            return false;
        const CombinedOutputSnapshot snapshot{
            source, output, aspect, flags, outputAspect,
            outputField40, outputField58, outputField68, true };
        const bool outputChanged = !g_lastCombinedOutput.valid ||
            snapshot.source != g_lastCombinedOutput.source ||
            snapshot.output != g_lastCombinedOutput.output ||
            snapshot.sourceAspect != g_lastCombinedOutput.sourceAspect ||
            snapshot.sourceFlags != g_lastCombinedOutput.sourceFlags ||
            snapshot.outputAspect != g_lastCombinedOutput.outputAspect ||
            snapshot.outputField58 != g_lastCombinedOutput.outputField58;
        g_lastCombinedOutput = snapshot;
#else
        const bool outputChanged = false;
#endif

        std::uint32_t aspectBits{};
        std::memcpy(&aspectBits, &aspect, sizeof(aspectBits));
        const std::uint32_t mode = aspectBits ^ (static_cast<std::uint32_t>(flags) << 1) ^
            (static_cast<std::uint32_t>(selector) << 9);
        const bool sourceModeChanged = g_lastCameraMode.exchange(mode, std::memory_order_relaxed) != mode;
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        g_logCombinedOutputForInvocation = sourceModeChanged || outputChanged;
        if (!g_logCombinedOutputForInvocation)
            return false;
#else
        if (!sourceModeChanged)
            return false;
#endif

        Log("Camera mode: source=0x", std::hex, source, std::dec,
            " rawSourceAspect=", rawSourceAspect, " rawSourceFlags=0x", std::hex,
            static_cast<int>(rawSourceFlags), std::dec,
            " writerInputXmm0=", writerInputXmm0,
            " cameraWorldFov(+0x230)=", cameraWorldFov,
            " cameraFirstPersonFov(+0x234)=", cameraFirstPersonFov,
            " aspect=", aspect, " flags=0x", std::hex, static_cast<int>(flags),
            " selector=0x", static_cast<int>(selector), std::dec,
            " outputFOV(before)=", outputFov, " outputAspect(before)=", outputAspect, ".");
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        Log("Combined output PRE: source=0x", std::hex, source, " output=0x", output, std::dec,
            " outputFov=", outputFov, " outputAspect=", outputAspect,
            " output+0x40=0x", std::hex, outputField40,
            " output+0x58=0x", outputField58, " output+0x68=0x", outputField68, std::dec, ".");
#endif
        return true;
    }

#ifdef GAMEPLAY_ONE_SHOT_CINEMATIC_TRIGGER
    bool WriteFlagsOnly(std::uintptr_t source, std::uint8_t flags)
    {
        if (!IsWritable(source + kFlagsOffset, sizeof(flags))) return false;
        std::memcpy(reinterpret_cast<void*>(source + kFlagsOffset), &flags, sizeof(flags));
        return true;
    }

    DWORD WINAPI OneShotTriggerLoop(void*)
    {
        bool previousF7 = false;
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        bool previousF6 = false;
        bool previousF8 = false;
        bool previousF9 = false;
#endif
        while (true) {
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            const bool f6 = (GetAsyncKeyState(VK_F6) & 0x8000) != 0;
#endif
            const bool f7 = (GetAsyncKeyState(VK_F7) & 0x8000) != 0;
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            const bool f8 = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
            const bool f9 = (GetAsyncKeyState(VK_F9) & 0x8000) != 0;
            if (f6 && !previousF6)
                Log("Combined diagnostic marker: native-wrong cinematic state.");
#endif
            if (f7 && !previousF7) {
                if (g_readyForOneShot.load(std::memory_order_acquire)) {
                    g_oneShotCinematicTriggerArmed.store(true, std::memory_order_release);
                    Log("Diagnostic one-shot cinematic trigger armed from validated READY state.");
                } else {
                    Log("Diagnostic F7 refused: validated 32:9/0x5 Waiting state is not ready.");
                }
            }
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            if (f8 && !previousF8) {
                const auto source = g_lastCameraSource.load(std::memory_order_acquire);
                float currentAspect = 0.0f;
                DEVMODE display{ .dmSize = sizeof(DEVMODE) };
                const bool displayReady = EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &display) && display.dmPelsHeight != 0;
                const float displayAspect = displayReady
                    ? static_cast<float>(display.dmPelsWidth) / static_cast<float>(display.dmPelsHeight) : 0.0f;
                const bool applied = source && displayReady &&
                    g_state.load(std::memory_order_acquire) == ReplayState::Complete &&
                    SafeRead(source + kAspectOffset, currentAspect) &&
                    std::fabs(currentAspect - kNativeAspect) <= 0.001f &&
                    IsWritable(source + kAspectOffset, sizeof(displayAspect));
                if (applied) {
                    std::memcpy(reinterpret_cast<void*>(source + kAspectOffset), &displayAspect, sizeof(displayAspect));
                    g_suppressRearmAfterManualRestore.store(true, std::memory_order_release);
                    Log("Combined diagnostic F8: native aspect restore source=0x", std::hex, source,
                        std::dec, " aspect=", currentAspect, " -> ", displayAspect,
                        " flags preserved.");
                } else {
                    Log("Combined diagnostic F8 refused: Complete + constrained 16:9 state was not confirmed.");
                }
            }
            if (f9 && !previousF9)
                Log("Combined diagnostic marker: observation window.");
            previousF6 = f6;
#endif
            previousF7 = f7;
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            previousF8 = f8;
            previousF9 = f9;
#endif
            Sleep(10);
        }
    }
#endif

    void LogTransitionSnapshot(const char* stage, std::uint64_t sequence, std::uintptr_t source)
    {
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) || !SafeRead(source + kFlagsOffset, flags)) {
            Log("Gameplay transition seq=", sequence, " stage=", stage,
                " source=0x", std::hex, source, std::dec, " readable=false");
            return;
        }

        Log("Gameplay transition seq=", sequence, " stage=", stage,
            " source=0x", std::hex, source, std::dec,
            " aspect=", aspect, " flags=0x", std::hex, static_cast<int>(flags), std::dec,
            " transitionState=", ReplayStateName(g_state.load(std::memory_order_relaxed)), ".");
    }

#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
    BOOL CALLBACK FindCurrentProcessWindow(HWND window, LPARAM parameter)
    {
        DWORD processId = 0;
        GetWindowThreadProcessId(window, &processId);
        if (processId == GetCurrentProcessId() && IsWindowVisible(window) && GetWindow(window, GW_OWNER) == nullptr) {
            *reinterpret_cast<HWND*>(parameter) = window;
            return FALSE;
        }
        return TRUE;
    }

    void LogResolutionChange()
    {
        DEVMODE display{ .dmSize = sizeof(DEVMODE) };
        const bool displayReady = EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &display) != FALSE;
        HWND window = nullptr;
        EnumWindows(FindCurrentProcessWindow, reinterpret_cast<LPARAM>(&window));
        RECT windowRect{};
        RECT clientRect{};
        const bool windowReady = window && GetWindowRect(window, &windowRect) && GetClientRect(window, &clientRect);
        ResolutionSnapshot current{
            displayReady ? display.dmPelsWidth : 0,
            displayReady ? display.dmPelsHeight : 0,
            windowReady ? windowRect.right - windowRect.left : 0,
            windowReady ? windowRect.bottom - windowRect.top : 0,
            windowReady ? clientRect.right - clientRect.left : 0,
            windowReady ? clientRect.bottom - clientRect.top : 0,
            displayReady || windowReady };
        if (!current.valid || (g_lastResolutionSnapshot.valid &&
            current.displayWidth == g_lastResolutionSnapshot.displayWidth &&
            current.displayHeight == g_lastResolutionSnapshot.displayHeight &&
            current.windowWidth == g_lastResolutionSnapshot.windowWidth &&
            current.windowHeight == g_lastResolutionSnapshot.windowHeight &&
            current.clientWidth == g_lastResolutionSnapshot.clientWidth &&
            current.clientHeight == g_lastResolutionSnapshot.clientHeight))
            return;

        g_lastResolutionSnapshot = current;
        Log("Resolution observation: display=", current.displayWidth, "x", current.displayHeight,
            " window=", current.windowWidth, "x", current.windowHeight,
            " client=", current.clientWidth, "x", current.clientHeight,
            " source=Win32 display/window bounds; engine render resolution not established.");
    }

    DWORD WINAPI ResolutionMonitorLoop(void*)
    {
        while (true) {
            LogResolutionChange();
            Sleep(250);
        }
    }

    void LogCombinedOutputPost(std::uint64_t sequence, std::uintptr_t source, std::uintptr_t output)
    {
        float outputFov = 0.0f;
        float outputAspect = 0.0f;
        std::uint64_t outputField40 = 0;
        std::uint64_t outputField58 = 0;
        std::uint64_t outputField68 = 0;
        if (!SafeRead(output + 0x30, outputFov) || !SafeRead(output + 0x5C, outputAspect) ||
            !SafeRead(output + 0x40, outputField40) || !SafeRead(output + 0x58, outputField58) ||
            !SafeRead(output + 0x68, outputField68)) {
            Log("Combined output POST seq=", sequence, " read-refused.");
            return;
        }
        Log("Combined output POST seq=", sequence, ": source=0x", std::hex, source,
            " output=0x", output, std::dec, " outputFov=", outputFov,
            " outputAspect=", outputAspect, " output+0x40=0x", std::hex, outputField40,
            " output+0x58=0x", outputField58, " output+0x68=0x", outputField68, std::dec, ".");
    }
#endif

    void ReplayManualTransitionOriginal(SafetyHookContext& context)
    {
        const auto source = context.rsi;
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        g_lastCameraSource.store(source, std::memory_order_release);
#endif
        const float fov = context.xmm0.f32[0];
        const auto previousSource = g_lastGameplayCameraSource.exchange(source, std::memory_order_acq_rel);
        const float previousFov = g_lastGameplayCameraFov.exchange(fov, std::memory_order_acq_rel);
        const bool sourceChanged = previousSource != 0 && previousSource != source;
        const bool materialFovJump = previousSource == source && std::isfinite(previousFov) &&
            std::isfinite(fov) && std::fabs(fov - previousFov) > kDialogueContextFovJump;
        if (sourceChanged || materialFovJump) {
            std::lock_guard dialogueLock(g_dialogueMutex);
            if (g_dialoguePhase != DialoguePhase::Inactive) {
                Log("Dialogue runtime invalidated by gameplay camera context change: sourceChanged=",
                    sourceChanged, " previousSource=0x", std::hex, previousSource,
                    " source=0x", source, std::dec, " previousFOV=", previousFov,
                    " currentFOV=", fov, ". Native pass-through until a new descent.");
                ResetDialogueRuntimeState();
            }
        }
#ifdef FOV_SETTINGS_TRACE_DIAGNOSTIC
        if (std::isfinite(fov) && fov > 1.0f && fov < 179.0f)
            LogGameplayFovChange(source, fov);
#endif
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) || !SafeRead(source + kFlagsOffset, flags)) return;
        const auto stateBeforeObservation = g_state.load(std::memory_order_acquire);
        const bool isOwnAutoRestore = stateBeforeObservation == ReplayState::Complete &&
            std::fabs(aspect - kNativeAspect) <= 0.001f &&
            g_lastAutoRestoreSource.load(std::memory_order_acquire) == source;
        // The restore observation arrives while the replay state is still
        // AppliedConstrainPass; g_lastAutoRestoreSource is assigned only after
        // the restore write below. Use the lifecycle state as the ownership
        // marker so the fix-owned native value cannot replace the last
        // externally observed ultrawide Auto aspect.
        const bool isPendingOwnAutoRestore = stateBeforeObservation == ReplayState::AppliedConstrainPass &&
            std::fabs(aspect - kNativeAspect) <= 0.001f;
        if (!isOwnAutoRestore && !isPendingOwnAutoRestore && IsValidAspect(aspect))
            g_lastObservedAspect.store(aspect, std::memory_order_release);
        else if (isPendingOwnAutoRestore)
            Log("Skipped fix-owned Auto restore in authoritative aspect cache; aspect=", aspect, ".");
#ifdef GAMEPLAY_ONE_SHOT_CINEMATIC_TRIGGER
        if (g_oneShotCinematicTriggerArmed.load(std::memory_order_acquire) &&
            g_state.load(std::memory_order_relaxed) == ReplayState::WaitingForAutomaticUpdate &&
            std::fabs(aspect - kWideAspect) <= 0.001f && flags == 0x5) {
            if (WriteFlagsOnly(source, 0x4)) {
                g_oneShotCinematicTriggerArmed.store(false, std::memory_order_release);
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
                g_readyForOneShot.store(false, std::memory_order_release);
#endif
                flags = 0x4;
                Log("Diagnostic one-shot cinematic trigger consumed at camera-writer boundary: source=0x",
                    std::hex, source, std::dec, " aspect=", aspect, " flags=0x5 -> 0x4.");
            } else {
                Log("Diagnostic one-shot cinematic trigger refused: flags field was not writable.");
            }
        }
#endif
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
        const auto currentState = g_state.load(std::memory_order_acquire);
        const bool ready = currentState == ReplayState::WaitingForAutomaticUpdate &&
            std::fabs(aspect - kWideAspect) <= 0.001f && flags == 0x5;
        g_readyForOneShot.store(ready, std::memory_order_release);
        if (ready && !g_readyMarkerLogged.exchange(true, std::memory_order_acq_rel))
            Log("Combined diagnostic READY FOR F7: source=0x", std::hex, source,
                std::dec, " aspect=", aspect, " flags=0x5 transitionState=WaitingForAutomaticUpdate.");
        if (!ready)
            g_readyMarkerLogged.store(false, std::memory_order_release);
#endif
        const bool modeChanged = LogCameraModeChange(source, context.rbx, fov, aspect, flags);
        const auto traceSequence = modeChanged
            ? g_transitionTraceSequence.fetch_add(1, std::memory_order_relaxed) + 1
            : 0;
        if (modeChanged) LogTransitionSnapshot("PRE", traceSequence, source);
        const auto logPost = [&]() {
            if (modeChanged) LogTransitionSnapshot("POST", traceSequence, source);
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            if (modeChanged && g_logCombinedOutputForInvocation)
                LogCombinedOutputPost(traceSequence, source, context.rbx);
#endif
        };

        if (std::fabs(aspect - kCinemaAspect) <= 0.001f && flags == 0x5) {
            // Preserve constrained aspect ratio for a real 21:9 output. The FOV source
            // is already correct; only the camera aspect state needs normalization.
            if (WriteAspectAndFlags(source, kNativeAspect, 0x5))
                Log("Normalized 21:9 camera aspect while preserving constrained aspect ratio.");
            logPost();
            return;
        }

        auto state = g_state.load(std::memory_order_relaxed);
        if (state == ReplayState::Complete && IsUltrawideAspect(aspect) && flags == 0x4) {
            // Cutscenes can rebuild the gameplay camera and restore the same broken
            // Auto state seen during startup. Arm the proven two-pass transition again.
            {
                std::lock_guard dialogueLock(g_dialogueMutex);
                ResetDialogueRuntimeState();
            }
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            if (g_suppressRearmAfterManualRestore.exchange(false, std::memory_order_acq_rel)) {
                Log("Combined diagnostic: manual native-aspect restore observed; automatic re-arm suppressed.");
            } else {
#endif
                g_state.store(ReplayState::WaitingForAutomaticUpdate, std::memory_order_relaxed);
                state = ReplayState::WaitingForAutomaticUpdate;
                Log("Gameplay camera was rebuilt; re-arming aspect transition.");
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            }
#endif
        }

        if (state == ReplayState::WaitingForAutomaticUpdate) {
            // The game has performed its own startup update. Recreate the observed
            // temporary constrained pass that occurs when the user applies 16:9.
            if (!IsUltrawideAspect(aspect) || flags != 0x4) {
                logPost();
                return;
            }
#ifdef GAMEPLAY_FIX_ATOMICITY_TEST
            ApplyGameplayAspectFixAtomic(source, "GameplayDetected", fov, aspect, flags);
            logPost();
            return;
#else
            if (WriteAspectAndFlags(source, aspect, 0x5)) {
                g_state.store(ReplayState::AppliedConstrainPass, std::memory_order_relaxed);
                Log("Replayed constrained pass: fov=", fov, " aspect=", aspect, " flags=0x5.");
            } else {
                Log("Replay refused: constrained-pass fields were not writable.");
            }
            logPost();
            return;
#endif
        }

#ifndef GAMEPLAY_FIX_ATOMICITY_TEST
        if (state == ReplayState::AppliedConstrainPass) {
            // On the following camera update, restore Auto's native aspect and flags.
            if (WriteAspectAndFlags(source, kNativeAspect, 0x4)) {
                g_state.store(ReplayState::Complete, std::memory_order_relaxed);
                g_lastAutoRestoreSource.store(source, std::memory_order_release);
                Log("Replayed Auto restore: fov=", fov, " aspect=", kNativeAspect, " flags=0x4.");
            } else {
                Log("Replay refused: Auto-restore fields were not writable.");
            }
        }
#endif
        logPost();
    }

#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
    bool TryDeferredGameplayReplay(SafetyHookContext& context)
    {
        if (!g_deferredGameplayReplayPending.load(std::memory_order_acquire)) return false;

        const auto source = static_cast<std::uintptr_t>(context.rsi);
        const float fov = context.xmm0.f32[0];
        const float targetFov = g_exitTargetFov.load(std::memory_order_acquire);
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) || !SafeRead(source + kFlagsOffset, flags)) {
            CancelDeferredGameplayReplay("camera-state-unreadable", source, fov, aspect, flags);
            return true;
        }

        bool dialogueActive = false;
        {
            std::lock_guard dialogueLock(g_dialogueMutex);
            dialogueActive = g_dialoguePhase != DialoguePhase::Inactive;
        }
        if (g_coordinator.load(std::memory_order_acquire) != CoordinatorState::Gameplay) {
            CancelDeferredGameplayReplay("coordinator-not-gameplay", source, fov, aspect, flags);
            return true;
        }
        if (dialogueActive) {
            CancelDeferredGameplayReplay("dialogue-active", source, fov, aspect, flags);
            return true;
        }

        const auto expectedSource = g_deferredGameplaySource.load(std::memory_order_acquire);
        if (expectedSource != 0 && expectedSource != source) {
            CancelDeferredGameplayReplay("source-changed", source, fov, aspect, flags);
            return true;
        }
        if (!std::isfinite(fov) || !std::isfinite(targetFov) ||
            std::fabs(fov - targetFov) > kRecoveryEpsilon) {
            CancelDeferredGameplayReplay("fov-left-target-tolerance", source, fov, aspect, flags);
            return true;
        }
        if (!IsUltrawideAspect(aspect) || flags != 0x4) {
            CancelDeferredGameplayReplay("gameplay-replay-not-applicable", source, fov, aspect, flags);
            return true;
        }

        const float previousFov = g_deferredGameplayLastFov.load(std::memory_order_acquire);
        if (std::isfinite(previousFov) && std::fabs(fov - previousFov) > kDialogueTransformEpsilon) {
            CancelDeferredGameplayReplay("fov-not-stable", source, fov, aspect, flags);
            return true;
        }
        if (expectedSource == 0)
            g_deferredGameplaySource.store(source, std::memory_order_release);
        g_deferredGameplayLastFov.store(fov, std::memory_order_release);

        auto samples = g_deferredGameplayStableSamples.load(std::memory_order_acquire);
        if (samples < kDeferredGameplayStableSamples) {
            samples = g_deferredGameplayStableSamples.fetch_add(1,
                std::memory_order_acq_rel) + 1;
            Log("StableSample ", samples, "/3: source=0x", std::hex, source, std::dec,
                " fov=", fov, " targetFov=", targetFov, " aspect=", aspect,
                " flags=0x", std::hex, static_cast<unsigned>(flags), std::dec,
                " elapsedUs=", (NowSteadyNs() - g_postExitTraceStartNs.load(std::memory_order_acquire)) / 1000, ".");
            if (samples < kDeferredGameplayStableSamples) return true;

            const auto stableSince = NowSteadyNs();
            g_deferredGameplayStableSinceNs.store(stableSince, std::memory_order_release);
            Log("StableStateConfirmed: source=0x", std::hex, source, std::dec,
                " fov=", fov, " aspect=", aspect, " flags=0x", std::hex,
                static_cast<unsigned>(flags), std::dec, ".");
        }
        const auto stableSince = g_deferredGameplayStableSinceNs.load(std::memory_order_acquire);
        if (NowSteadyNs() - stableSince < kDeferredGameplayDelayNs) return true;

        bool expectedPending = true;
        if (!g_deferredGameplayReplayPending.compare_exchange_strong(expectedPending, false,
            std::memory_order_acq_rel)) return true;
        g_deferredGameplayStableSamples.store(0, std::memory_order_release);
        Log("DeferredReplayApplied: source=0x", std::hex, source, std::dec,
            " fov=", fov, " aspect=", aspect, " flags=0x", std::hex,
            static_cast<unsigned>(flags), std::dec, " delayMs=",
            POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS,
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMICITY_TEST
            " mode=atomic.");
#else
            " mode=legacy.");
#endif
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMICITY_TEST
        if (WriteAspectAndFlags(source, kNativeAspect, 0x4)) {
            g_state.store(ReplayState::Complete, std::memory_order_release);
            g_lastAutoRestoreSource.store(source, std::memory_order_release);
            Log("AtomicReplayApplied: source=0x", std::hex, source, std::dec,
                " fov=", fov, " aspect=", kNativeAspect, " flags=0x4 delayMs=",
                POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_DELAY_MS, ".");
        } else {
            Log("AtomicReplayRefused: final aspect/flags fields were not writable.");
        }
#else
        g_state.store(ReplayState::WaitingForAutomaticUpdate, std::memory_order_release);
        ReplayManualTransitionOriginal(context);
#endif
        return true;
    }
#endif

    float CinematicHorPlus(float fov, float aspect)
    {
        constexpr float pi = 3.14159265358979323846f;
        const float half = fov * (pi / 360.0f);
        return 2.0f * std::atan(std::tan(half) * (aspect / kNativeAspect)) * (180.0f / pi);
    }

    bool IsExecutable(std::uintptr_t address)
    {
        MEMORY_BASIC_INFORMATION info{};
        if (!VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info))) return false;
        const auto protection = info.Protect & 0xFF;
        return protection == PAGE_EXECUTE || protection == PAGE_EXECUTE_READ ||
            protection == PAGE_EXECUTE_READWRITE || protection == PAGE_EXECUTE_WRITECOPY;
    }

    bool DecodeInstruction(std::uint8_t* address, ZydisDecodedInstruction& instruction,
        ZydisDecodedOperand* operands)
    {
        ZydisDecoder decoder{};
        return ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)) &&
            ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, address, 15, &instruction, operands));
    }

    bool IsCallRel32(std::uint8_t* address)
    {
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        return DecodeInstruction(address, instruction, operands) &&
            address[0] == 0xE8 &&
            instruction.mnemonic == ZYDIS_MNEMONIC_CALL && instruction.length == 5 &&
            instruction.operand_count_visible == 1 &&
            operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE;
    }

    bool ContainsBytes(std::uint8_t* start, std::size_t length,
        const std::uint8_t* bytes, std::size_t byteCount)
    {
        for (std::size_t offset = 0; offset + byteCount <= length; ++offset)
            if (std::memcmp(start + offset, bytes, byteCount) == 0) return true;
        return false;
    }

    std::uint8_t* ResolveRel32CallTarget(std::uint8_t* address)
    {
        if (!IsCallRel32(address)) return nullptr;
        std::int32_t displacement{};
        std::memcpy(&displacement, address + 1, sizeof(displacement));
        return address + 5 + displacement;
    }

    bool ValidateEnterBoundary(std::uint8_t* match, std::uint8_t*& callsite)
    {
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        if (!DecodeInstruction(match, instruction, operands) ||
            instruction.mnemonic != ZYDIS_MNEMONIC_MOVSS || instruction.length != 8 ||
            instruction.operand_count_visible < 2 ||
            operands[0].type != ZYDIS_OPERAND_TYPE_REGISTER ||
            operands[0].reg.value != ZYDIS_REGISTER_XMM0 ||
            operands[1].type != ZYDIS_OPERAND_TYPE_MEMORY ||
            operands[1].mem.base != ZYDIS_REGISTER_RIP) return false;
        callsite = match + 8;
        return IsCallRel32(callsite) &&
            ContainsBytes(match, 96, kEnterVcallPair, sizeof(kEnterVcallPair));
    }

    bool ValidateExitBoundary(std::uint8_t* match, std::uint8_t*& callsite)
    {
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        if (!DecodeInstruction(match, instruction, operands) ||
            instruction.mnemonic != ZYDIS_MNEMONIC_MOVSS || instruction.length != 5 ||
            instruction.operand_count_visible < 2 ||
            operands[0].type != ZYDIS_OPERAND_TYPE_REGISTER ||
            operands[0].reg.value != ZYDIS_REGISTER_XMM0 ||
            operands[1].type != ZYDIS_OPERAND_TYPE_MEMORY ||
            operands[1].mem.base != ZYDIS_REGISTER_RDI ||
            !operands[1].mem.disp.has_displacement || operands[1].mem.disp.value != 0x38) return false;
        callsite = match + 5;
        return IsCallRel32(callsite) &&
            ContainsBytes(match, 96, kExitVcallPair, sizeof(kExitVcallPair));
    }

    bool ValidateIndexedExitBoundary(std::uint8_t* match, std::uint8_t*& callsite)
    {
        constexpr std::size_t kMovssOffset = 4;
        constexpr std::size_t kCallOffset = 10;
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        if (!DecodeInstruction(match + kMovssOffset, instruction, operands) ||
            instruction.mnemonic != ZYDIS_MNEMONIC_MOVSS || instruction.length != 6 ||
            instruction.operand_count_visible < 2 ||
            operands[0].type != ZYDIS_OPERAND_TYPE_REGISTER ||
            operands[0].reg.value != ZYDIS_REGISTER_XMM0 ||
            operands[1].type != ZYDIS_OPERAND_TYPE_MEMORY ||
            operands[1].mem.base != ZYDIS_REGISTER_RBX ||
            operands[1].mem.index != ZYDIS_REGISTER_RAX ||
            operands[1].mem.scale != 4 ||
            !operands[1].mem.disp.has_displacement ||
            operands[1].mem.disp.value != 0x38) return false;
        callsite = match + kCallOffset;
        return IsCallRel32(callsite) &&
            ContainsBytes(match, 96, kExitVcallPair, sizeof(kExitVcallPair));
    }

    bool ResolveCinematicAspectStore(std::uint8_t*& store)
    {
        const auto matches = Memory::PatternScanAll(g_executable, kCinematicAspectSetterSignature);
        if (matches.size() != 1) {
            Log("Cinematic aspect signature rejected: matches=", matches.size(), ".");
            return false;
        }
        const auto candidate = matches.front() + 0x19;
        if (!IsExecutable(reinterpret_cast<std::uintptr_t>(candidate)) ||
            std::memcmp(candidate, kCinematicStorePrefix, sizeof(kCinematicStorePrefix)) != 0 ||
            std::memcmp(candidate + kCinematicImmediateOffset, kCinematicOriginalImmediate,
                sizeof(kCinematicOriginalImmediate)) != 0) return false;

        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        if (!DecodeInstruction(candidate, instruction, operands) ||
            instruction.mnemonic != ZYDIS_MNEMONIC_MOV || instruction.length != kCinematicStoreInstructionLength ||
            instruction.operand_count_visible < 2 || operands[0].type != ZYDIS_OPERAND_TYPE_MEMORY ||
            operands[0].mem.base != ZYDIS_REGISTER_RAX || !operands[0].mem.disp.has_displacement ||
            operands[0].mem.disp.value != kAspectOffset || operands[1].type != ZYDIS_OPERAND_TYPE_IMMEDIATE ||
            operands[1].imm.value.u != 0x3FE38E39) return false;
        store = candidate;
        Log("Cinematic aspect signature validated at RVA=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(store) - reinterpret_cast<std::uintptr_t>(g_executable), std::dec, ".");
        return true;
    }

    bool ResolveCinematicFovCallsites(std::uint8_t*& enter, std::uint8_t*& exit)
    {
        const auto enterMatches = Memory::PatternScanAll(g_executable, kCinematicEnterSignature);
        auto exitMatches = Memory::PatternScanAll(g_executable, kCinematicExitSignature);
        bool indexedExit = false;
        if (exitMatches.empty()) {
            exitMatches = Memory::PatternScanAll(g_executable, kCinematicExitIndexedSignature);
            indexedExit = true;
        }
        if (enterMatches.size() != 1 || exitMatches.size() != 1) {
            Log("Cinematic FOV signatures rejected: enterMatches=", enterMatches.size(),
                " exitMatches=", exitMatches.size(), ".");
            return false;
        }
        if (!ValidateEnterBoundary(enterMatches.front(), enter) ||
            (indexedExit
                ? !ValidateIndexedExitBoundary(exitMatches.front(), exit)
                : !ValidateExitBoundary(exitMatches.front(), exit))) return false;
        const auto enterTarget = ResolveRel32CallTarget(enter);
        const auto exitTarget = ResolveRel32CallTarget(exit);
        if (!enterTarget || enterTarget != exitTarget || !IsExecutable(reinterpret_cast<std::uintptr_t>(enterTarget))) {
            Log("Cinematic FOV consumer target validation failed.");
            return false;
        }
        Log("Cinematic FOV signatures validated at ENTER RVA=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(enter) - reinterpret_cast<std::uintptr_t>(g_executable),
            " EXIT RVA=0x", reinterpret_cast<std::uintptr_t>(exit) - reinterpret_cast<std::uintptr_t>(g_executable),
            " consumer=0x", reinterpret_cast<std::uintptr_t>(enterTarget), std::dec, ".");
        return true;
    }

    bool InstallCinematicAspect(std::uint8_t* store)
    {
        g_cinematicAspectStore = store;
        if (!IsExecutable(reinterpret_cast<std::uintptr_t>(g_cinematicAspectStore)) ||
            std::memcmp(g_cinematicAspectStore, kCinematicStorePrefix, sizeof(kCinematicStorePrefix)) != 0 ||
            std::memcmp(g_cinematicAspectStore + kCinematicImmediateOffset,
                kCinematicOriginalImmediate, sizeof(kCinematicOriginalImmediate)) != 0) return false;
        std::memcpy(g_cinematicOriginalImmediate, g_cinematicAspectStore + kCinematicImmediateOffset,
            sizeof(g_cinematicOriginalImmediate));
        g_cinematicAspectStoreHook = safetyhook::create_mid(
            g_cinematicAspectStore, ApplyCinematicAspectStore);
        if (!g_cinematicAspectStoreHook) return false;
        g_cinematicAspectPatched = true;
        return true;
    }

    void RestoreCinematicAspect()
    {
        g_cinematicAspectStoreHook.reset();
        g_cinematicAspectPatched = false;
    }

    void TraceCinematicEnter(SafetyHookContext& context)
    {
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
        CancelDeferredGameplayReplay("new-cinematic-enter");
#endif
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
        ResetAtomicExitHandoff("new-cinematic-enter");
#endif
        const bool cinematicFovEnabled = CinematicAspectOverrideEnabled();
        const auto policy = g_runtimeCinematicPolicy.load(std::memory_order_acquire);
        if (cinematicFovEnabled) {
            bool expected = false;
            if (!g_cinematicFovApplied.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) return;
        }
        const float before = context.xmm0.f32[0];
        const float aspect = ResolveCinematicAspect(0);
        const float after = cinematicFovEnabled && std::isfinite(before) && std::isfinite(aspect) &&
            before > 1.0f && before < 179.0f && aspect > 1.0f
            ? CinematicHorPlus(before, aspect) : before;
        if (std::isfinite(after) && after > 1.0f && after < 179.0f) {
            context.xmm0.f32[0] = after;
        }
        g_coordinator.store(CoordinatorState::CinematicActive, std::memory_order_release);
        Log("Global cinematic ENTER: aspect=", aspect, " authoredFov=", before,
            " transformedFov=", context.xmm0.f32[0],
            " aspectPolicy=", CinematicAspectPolicyName(policy), ".",
            " Gameplay replay suppressed.");
    }

    void TraceCinematicExit(SafetyHookContext& context)
    {
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
        CancelDeferredGameplayReplay("new-cinematic-exit");
#endif
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
        ResetAtomicExitHandoff("new-cinematic-exit");
#endif
        g_exitTargetFov.store(context.xmm0.f32[0], std::memory_order_release);
        g_postExitTraceWriterCount.store(0, std::memory_order_release);
        g_postExitTraceSequence.fetch_add(1, std::memory_order_acq_rel);
        g_postExitTraceStartNs.store(std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count(), std::memory_order_release);
        g_postExitTraceArmed.store(true, std::memory_order_release);
        g_cinematicFovApplied.store(false, std::memory_order_release);
        g_coordinator.store(CoordinatorState::CinematicExiting, std::memory_order_release);
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
        if (g_config.gameplayEnabled) ArmAtomicExitHandoff();
#endif
        Log("Global cinematic EXIT: nativeTargetFov=", context.xmm0.f32[0],
            " postExitTrace=armed sequence=", g_postExitTraceSequence.load(std::memory_order_acquire),
            ". Gameplay replay suppressed until native recovery.");
    }

    float TransformDialogueProjectionSample(float incoming, float baseline, float target)
    {
        constexpr float kNativeDialogueTarget = 70.0f;
        constexpr float kProjectionIntervalTolerance = 0.25f;
        const float intervalLow = baseline < kNativeDialogueTarget ? baseline : kNativeDialogueTarget;
        const float intervalHigh = baseline > kNativeDialogueTarget ? baseline : kNativeDialogueTarget;
        if (incoming < intervalLow - kProjectionIntervalTolerance ||
            incoming > intervalHigh + kProjectionIntervalTolerance) return incoming;
        const float baselineProjection = std::tan(baseline * 0.5f * 0.01745329251994329577f);
        const float nativeTargetProjection = std::tan(kNativeDialogueTarget * 0.5f * 0.01745329251994329577f);
        const float incomingProjection = std::tan(incoming * 0.5f * 0.01745329251994329577f);
        const float targetProjection = std::tan(target * 0.5f * 0.01745329251994329577f);
        const float nativeSpan = nativeTargetProjection - baselineProjection;
        if (!std::isfinite(nativeSpan) || std::fabs(nativeSpan) <= 1.0e-6f) return incoming;

        const float progress = std::clamp(
            (incomingProjection - baselineProjection) / nativeSpan, 0.0f, 1.0f);
        const float outputProjection = baselineProjection +
            progress * (targetProjection - baselineProjection);
        const float output = 2.0f * std::atan(outputProjection) /
            0.01745329251994329577f;
        return std::isfinite(output) ? output : incoming;
    }

    float TransformDialogueExitSample(float incoming, float baseline, float target, float exitStart)
    {
        constexpr float kRadiansPerDegree = 0.01745329251994329577f;
        const float startProjection = std::tan(exitStart * 0.5f * kRadiansPerDegree);
        const float baselineProjection = std::tan(baseline * 0.5f * kRadiansPerDegree);
        const float incomingProjection = std::tan(incoming * 0.5f * kRadiansPerDegree);
        const float targetProjection = std::tan(target * 0.5f * kRadiansPerDegree);
        const float nativeSpan = baselineProjection - startProjection;
        if (!std::isfinite(nativeSpan) || std::fabs(nativeSpan) <= 1.0e-6f) return incoming;

        const float progress = std::clamp(
            (incomingProjection - startProjection) / nativeSpan, 0.0f, 1.0f);
        const float outputProjection = targetProjection +
            progress * (baselineProjection - targetProjection);
        const float output = 2.0f * std::atan(outputProjection) / kRadiansPerDegree;
        return std::isfinite(output) ? output : incoming;
    }

    float DialogueAdaptiveTarget(float baseline)
    {
        constexpr float kNativeDialogueTarget = 70.0f;
        constexpr float kNativeReferenceGameplay = 90.0f;
        constexpr float kRadiansPerDegree = 0.01745329251994329577f;
        const float nativeZoom =
            std::tan(kNativeReferenceGameplay * 0.5f * kRadiansPerDegree) /
            std::tan(kNativeDialogueTarget * 0.5f * kRadiansPerDegree);
        const float projection = std::tan(baseline * 0.5f * kRadiansPerDegree) / nativeZoom;
        const float output = 2.0f * std::atan(projection) / kRadiansPerDegree;
        return std::isfinite(output) ? output : baseline;
    }

    float DialogueReducedTarget(float baseline)
    {
        constexpr float kNativeDialogueTarget = 70.0f;
        constexpr float kNativeReferenceGameplay = 90.0f;
        constexpr float kRadiansPerDegree = 0.01745329251994329577f;
        const float nativeZoom =
            std::tan(kNativeReferenceGameplay * 0.5f * kRadiansPerDegree) /
            std::tan(kNativeDialogueTarget * 0.5f * kRadiansPerDegree);
        const float projection = std::tan(baseline * 0.5f * kRadiansPerDegree) /
            std::sqrt(nativeZoom);
        const float output = 2.0f * std::atan(projection) / kRadiansPerDegree;
        return std::isfinite(output) ? output : baseline;
    }

    void TraceDialogueBoundary(SafetyHookContext& context)
    {
        const float incoming = context.xmm6.f32[0];
        if (!std::isfinite(incoming) || incoming <= 1.0f || incoming >= 179.0f) return;

        std::lock_guard lock(g_dialogueMutex);
        if (g_coordinator.load(std::memory_order_acquire) != CoordinatorState::Gameplay) {
            ResetDialogueRuntimeState();
            return;
        }
        const auto policy = g_runtimeDialoguePolicy.load(std::memory_order_acquire);
        if (policy == DialogueZoomPolicy::Native) {
            ResetDialogueRuntimeState();
            return;
        }
        const float previous = g_dialoguePrevious;
        const bool descending = std::isfinite(previous) && incoming < previous - kDialogueTransformEpsilon;
        const bool ascending = std::isfinite(previous) && incoming > previous + kDialogueTransformEpsilon;

        if (g_dialoguePhase == DialoguePhase::Inactive) {
            if (!std::isfinite(g_dialogueBaseline)) {
                g_dialogueBaseline = incoming;
                g_dialoguePhase = DialoguePhase::Candidate;
                g_dialogueExitIncomingStart = std::numeric_limits<float>::quiet_NaN();
                Log("Dialogue candidate captured: provisionalBaselineG=", g_dialogueBaseline,
                    " policy=", DialogueZoomPolicyName(policy), ". No transform applied.");
            } else if (descending) {
                g_dialogueBaseline = previous;
                g_dialoguePhase = DialoguePhase::Active;
                g_dialogueExitIncomingStart = std::numeric_limits<float>::quiet_NaN();
                Log("Dialogue lifecycle restarted after gameplay descent: baselineG=", g_dialogueBaseline, ".");
            }
        } else if (g_dialoguePhase == DialoguePhase::Candidate && descending) {
            g_dialogueBaseline = previous;
            g_dialoguePhase = DialoguePhase::Active;
            g_dialogueExitIncomingStart = std::numeric_limits<float>::quiet_NaN();
            Log("Dialogue lifecycle started after confirmed native descent: baselineG=", g_dialogueBaseline,
                " policy=", DialogueZoomPolicyName(policy), ".");
        } else if (g_dialoguePhase == DialoguePhase::Active && ascending) {
            g_dialoguePhase = DialoguePhase::Exiting;
            g_dialogueExitIncomingStart = incoming;
            Log("Dialogue lifecycle exiting: incoming=", incoming, " baselineG=", g_dialogueBaseline, ".");
        }

        float output = incoming;
        if (g_dialoguePhase == DialoguePhase::Exiting && ascending &&
            std::isfinite(g_dialogueBaseline) &&
            std::fabs(incoming - g_dialogueBaseline) <= kDialogueRecoveryEpsilon) {
            Log("Dialogue lifecycle recovered: baselineG=", g_dialogueBaseline, ". Native recovery complete.");
            ResetDialogueRuntimeState();
            output = incoming;
        }
        else if ((policy == DialogueZoomPolicy::Adaptive || policy == DialogueZoomPolicy::Reduced) &&
            (g_dialoguePhase == DialoguePhase::Active || g_dialoguePhase == DialoguePhase::Exiting) &&
            std::isfinite(g_dialogueBaseline)) {
            const float target = policy == DialogueZoomPolicy::Adaptive
                ? DialogueAdaptiveTarget(g_dialogueBaseline)
                : DialogueReducedTarget(g_dialogueBaseline);
            output = g_dialoguePhase == DialoguePhase::Exiting &&
                std::isfinite(g_dialogueExitIncomingStart)
                ? TransformDialogueExitSample(incoming, g_dialogueBaseline, target,
                    g_dialogueExitIncomingStart)
                : TransformDialogueProjectionSample(incoming, g_dialogueBaseline, target);
        }
        else if (policy == DialogueZoomPolicy::Disabled &&
            (g_dialoguePhase == DialoguePhase::Active || g_dialoguePhase == DialoguePhase::Exiting) &&
            std::isfinite(g_dialogueBaseline)) {
            output = g_dialogueBaseline;
        }

        if (std::isfinite(output) && output > 1.0f && output < 179.0f)
            context.xmm1.f32[0] = output;
        g_dialoguePrevious = incoming;
    }

    bool InstallDialogueBoundary()
    {
        const auto matches = Memory::PatternScanAll(g_executable, kDialogueBoundarySignature);
        if (matches.size() != 1) {
            Log("Dialogue boundary resolver rejected: matches=", matches.size(), ". Native pass-through retained.");
            return false;
        }
        auto* hook = matches.front() + kDialogueBoundaryHookOffset;
        constexpr std::uint8_t expected[] = { 0xFF, 0x90, 0x08, 0x06, 0x00, 0x00 };
        if (std::memcmp(hook, expected, sizeof(expected)) != 0) {
            Log("Dialogue boundary instruction contract rejected. Native pass-through retained.");
            return false;
        }
        g_dialogueBoundaryHook = safetyhook::create_mid(hook, TraceDialogueBoundary);
        if (!g_dialogueBoundaryHook || !g_dialogueBoundaryHook.enable()) {
            g_dialogueBoundaryHook.reset();
            Log("Dialogue boundary hook setup failed. Native pass-through retained.");
            return false;
        }
        Log("Dialogue boundary hook installed: RVA=0x", std::hex,
            reinterpret_cast<std::uintptr_t>(hook) - reinterpret_cast<std::uintptr_t>(g_executable),
            std::dec, " policy=", DialogueZoomPolicyName(g_runtimeDialoguePolicy.load(std::memory_order_acquire)), ".");
        return true;
    }

#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
    bool TryAtomicExitHandoff(SafetyHookContext& context)
    {
        if (!g_atomicExitHandoffPending.load(std::memory_order_acquire)) return false;
        if (g_coordinator.load(std::memory_order_acquire) != CoordinatorState::CinematicExiting) {
            ResetAtomicExitHandoff("coordinator-not-exiting");
            return false;
        }

        {
            std::lock_guard dialogueLock(g_dialogueMutex);
            if (g_dialoguePhase != DialoguePhase::Inactive) {
                ResetAtomicExitHandoff("dialogue-active");
                return false;
            }
        }

        const auto source = static_cast<std::uintptr_t>(context.rsi);
        const float currentFov = context.xmm0.f32[0];
        const float targetFov = g_exitTargetFov.load(std::memory_order_acquire);
        const auto expectedSource = g_atomicExitHandoffSource.load(std::memory_order_acquire);
        if (expectedSource != 0 && expectedSource != source) {
            ResetAtomicExitHandoff("source-changed");
            return false;
        }
        if (!std::isfinite(currentFov) || !std::isfinite(targetFov)) {
            ResetAtomicExitHandoff("fov-unreadable");
            return false;
        }

        const float previousFov = g_atomicExitHandoffPreviousFov.load(std::memory_order_acquire);
        if (expectedSource == 0) {
            g_atomicExitHandoffSource.store(source, std::memory_order_release);
            g_atomicExitHandoffPreviousFov.store(currentFov, std::memory_order_release);
            return false;
        }
        g_atomicExitHandoffPreviousFov.store(currentFov, std::memory_order_release);
        if (!std::isfinite(previousFov) ||
            currentFov >= previousFov - kDialogueTransformEpsilon ||
            currentFov <= targetFov + kRecoveryEpsilon) return false;

        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) ||
            !SafeRead(source + kFlagsOffset, flags)) {
            ResetAtomicExitHandoff("camera-state-unreadable");
            return false;
        }

        bool expectedPending = true;
        if (!g_atomicExitHandoffPending.compare_exchange_strong(expectedPending, false,
            std::memory_order_acq_rel)) return false;
        ApplyGameplayAspectFixAtomic(source, "RecoveryStart", currentFov, aspect, flags);
        return true;
    }
#endif

    void ReplayManualTransition(SafetyHookContext& context)
    {
        ObservePostExitRawWriterEntry(context);
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST
        if (TryAtomicExitHandoff(context)) return;
#endif
        const auto state = g_coordinator.load(std::memory_order_acquire);
        if (state == CoordinatorState::CinematicActive) {
            return;
        }
        if (state == CoordinatorState::CinematicExiting) {
            const auto source = static_cast<std::uintptr_t>(context.rsi);
            std::uint8_t flags{};
            SafeRead(source + kFlagsOffset, flags);
            const float currentFov = context.xmm0.f32[0];
            const float targetFov = g_exitTargetFov.load(std::memory_order_acquire);
            const float delta = std::fabs(currentFov - targetFov);
            if (flags == 0x04 && std::isfinite(currentFov) && std::isfinite(targetFov) && delta <= kRecoveryEpsilon) {
                g_coordinator.store(CoordinatorState::Gameplay, std::memory_order_release);
                {
                    std::lock_guard dialogueLock(g_dialogueMutex);
                    ResetDialogueRuntimeState();
                }
                Log("Global coordinator: native recovery complete; delta=", delta,
                    " target=", targetFov, " current=", currentFov,
                    ". Same writer invocation returned without replay.");
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
#if defined(POST_CINEMATIC_GAMEPLAY_REPLAY_AT_RECOVERY_TEST) && \
    !defined(POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST)
                if (g_config.gameplayEnabled) {
                    const auto source = static_cast<std::uintptr_t>(context.rsi);
                    std::uint8_t currentFlags{};
                    float currentAspect = 0.0f;
                    if (SafeRead(source + kAspectOffset, currentAspect) &&
                        SafeRead(source + kFlagsOffset, currentFlags) &&
                        WriteAspectAndFlags(source, kNativeAspect, 0x4)) {
                        g_state.store(ReplayState::Complete, std::memory_order_release);
                        g_lastAutoRestoreSource.store(source, std::memory_order_release);
                        Log("AtomicReplayApplied: phase=RecoveryComplete source=0x", std::hex,
                            source, std::dec, " fov=", currentFov, " aspect=", kNativeAspect,
                            " flags=0x4 previousAspect=", currentAspect, " previousFlags=0x",
                            std::hex, static_cast<unsigned>(currentFlags), std::dec, ".");
                    } else {
                        Log("AtomicReplayRefused: phase=RecoveryComplete final aspect/flags fields were not writable.");
                    }
                }
#elif !defined(POST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST)
                if (g_config.gameplayEnabled) ArmDeferredGameplayReplay(targetFov);
#endif
#endif
            }
            return;
        }
#ifdef POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST
        if (TryDeferredGameplayReplay(context)) return;
#endif
        if (!g_config.gameplayEnabled) {
            float aspect = 0.0f;
            const auto source = static_cast<std::uintptr_t>(context.rsi);
            if (SafeRead(source + kAspectOffset, aspect) && IsValidAspect(aspect))
                g_lastObservedAspect.store(aspect, std::memory_order_release);
            return;
        }
        ReplayManualTransitionOriginal(context);
    }

    bool VerifyExecutableAndInstruction()
    {
        const auto* base = reinterpret_cast<const std::uint8_t*>(g_executable);
        const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
        const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE) return false;

        const auto* section = IMAGE_FIRST_SECTION(nt);
        const std::uint8_t* textStart = nullptr;
        std::size_t textSize = 0;
        for (std::uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
            if (std::memcmp(section->Name, ".text", 5) == 0) {
                textStart = base + section->VirtualAddress;
                textSize = section->Misc.VirtualSize;
                break;
            }
        }
        if (!textStart || textSize < sizeof(kCameraWriterSignature)) return false;

        std::uint8_t* match = nullptr;
        std::size_t matches = 0;
        for (std::size_t offset = 0; offset <= textSize - sizeof(kCameraWriterSignature); ++offset) {
            const auto* candidate = textStart + offset;
            bool matchesSignature = true;
            for (std::size_t i = 0; i < sizeof(kCameraWriterSignature); ++i) {
                // The 32-bit relative target of JNZ may move when surrounding code changes.
                if ((i < 17 || i >= 21) && candidate[i] != kCameraWriterSignature[i]) {
                    matchesSignature = false;
                    break;
                }
            }
            if (matchesSignature) {
                match = const_cast<std::uint8_t*>(candidate);
                ++matches;
            }
        }
        if (matches != 1) {
            Log("Camera-writer signature rejected: matches=", matches, ".");
            return false;
        }

        ZydisDecoder decoder{};
        ZydisDecodedInstruction instruction{};
        // DecodeFull clears every possible operand slot, not just visible operands.
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        auto* target = match + kFovWriteOffsetInSignature;
        const bool verified = ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)) &&
            ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, target, 15, &instruction, operands)) &&
            instruction.mnemonic == ZYDIS_MNEMONIC_MOVSS && instruction.operand_count_visible >= 2 &&
            operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY && operands[1].type == ZYDIS_OPERAND_TYPE_REGISTER &&
            operands[0].mem.base == ZYDIS_REGISTER_RBX && operands[0].mem.disp.has_displacement &&
            operands[0].mem.disp.value == 0x30 && operands[1].reg.value == ZYDIS_REGISTER_XMM0;
        if (verified) {
            g_fovWriteAddress = target;
            Log("Camera-writer signature validated at RVA=0x", std::hex,
                reinterpret_cast<std::uintptr_t>(target) - reinterpret_cast<std::uintptr_t>(base), std::dec, ".");
        }
        return verified;
    }

    void HotkeyLoop()
    {
        bool previousDialogueKey = false;
        bool previousCinematicKey = false;
        for (;;) {
            const bool dialogueKey = (GetAsyncKeyState(g_config.dialogueCycleKey) & 0x8000) != 0;
            const bool cinematicKey = (GetAsyncKeyState(g_config.cinematicCycleKey) & 0x8000) != 0;
            if (g_config.hotkeysEnabled) {
                if (dialogueKey && !previousDialogueKey) {
                    const auto policy = NextDialogueZoomPolicy(
                        g_runtimeDialoguePolicy.load(std::memory_order_acquire));
                    g_runtimeDialoguePolicy.store(policy, std::memory_order_release);
                    if (PersistConfigValue(g_configPath, "Dialogue", "Zoom", DialogueZoomPolicyName(policy)))
                        Log("Hotkey ", HotkeyName(g_config.dialogueCycleKey), ": Dialogue.Zoom=", DialogueZoomPolicyName(policy), " persisted.");
                    else
                        Log("Hotkey ", HotkeyName(g_config.dialogueCycleKey), ": Dialogue.Zoom=", DialogueZoomPolicyName(policy), " active; persistence failed.");
                }
                if (cinematicKey && !previousCinematicKey) {
                    const auto policy = NextCinematicAspectPolicy(
                        g_runtimeCinematicPolicy.load(std::memory_order_acquire));
                    g_runtimeCinematicPolicy.store(policy, std::memory_order_release);
                    if (PersistConfigValue(g_configPath, "Cinematics", "AspectRatio", CinematicAspectPolicyName(policy)))
                        Log("Hotkey ", HotkeyName(g_config.cinematicCycleKey), ": Cinematics.AspectRatio=", CinematicAspectPolicyName(policy),
                            " persisted for next cinematic.");
                    else
                        Log("Hotkey ", HotkeyName(g_config.cinematicCycleKey), ": Cinematics.AspectRatio=", CinematicAspectPolicyName(policy),
                            " active for next cinematic; persistence failed.");
                }
            }
            previousDialogueKey = dialogueKey;
            previousCinematicKey = cinematicKey;
            Sleep(50);
        }
    }

    DWORD WINAPI Initialize(void*)
    {
        WCHAR modulePath[MAX_PATH]{};
        GetModuleFileNameW(g_module, modulePath, MAX_PATH);
        const auto moduleDirectory = std::filesystem::path(modulePath).remove_filename();
        const auto logPath = moduleDirectory / "STALKER2CameraTweaks.log";
        const auto configPath = moduleDirectory / "STALKER2CameraTweaks.ini";
        g_configPath = configPath;
        std::ofstream(logPath, std::ios::out | std::ios::trunc).close();
        try {
            g_logger = spdlog::basic_logger_mt("STALKER2CameraTweaks", logPath.string(), true);
            g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
            g_logger->flush_on(spdlog::level::info);
        } catch (...) { return 0; }

        try {
            std::string modHash;
            std::string gameHash;
            WCHAR executablePath[MAX_PATH]{};
            GetModuleFileNameW(nullptr, executablePath, MAX_PATH);
            const bool modHashAvailable = ComputeSha256(modulePath, modHash);
            const bool gameHashAvailable = ComputeSha256(executablePath, gameHash);
            Log("Runtime identity: modSha256=", modHashAvailable ? modHash : "unavailable",
                " gameSha256=", gameHashAvailable ? gameHash : "unavailable", ".");
#ifdef POST_EXIT_CAMERA_FOV_WRITE_OWNER_TRACE
            if (!gameHashAvailable || !InstallPostExitFovWriteOwnerTrace(gameHash))
                Log("Post-EXIT FOV physical-write trace: NOT_INSTALLED; production behavior unchanged.");
#endif
#ifdef POST_EXIT_FOV_PRODUCER_STORE_TRACE
            if (!gameHashAvailable || !InstallPostExitProducerStoreTrace(gameHash))
                Log("Post-EXIT FOV producer store trace: NOT_INSTALLED; production behavior unchanged.");
#endif
#ifdef POST_EXIT_FOV_STATE_CONSUMER_TRACE
            if (!gameHashAvailable || !InstallPostExitFovConsumerTrace(gameHash))
                Log("Post-EXIT FOV consumer trace: NOT_INSTALLED; production behavior unchanged.");
#endif
            if (!LoadFeatureConfig(configPath))
                Log("Configuration unavailable; using defaults with all fixes enabled.");
            g_runtimeCinematicPolicy.store(g_config.cinematicAspectPolicy, std::memory_order_release);
            g_runtimeDialoguePolicy.store(g_config.dialogueZoomPolicy, std::memory_order_release);
            Log("Configuration: Gameplay.Enabled=", g_config.gameplayEnabled,
                " Cinematic.AspectRatio=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy),
                " Dialogue.Zoom=", DialogueZoomPolicyName(g_config.dialogueZoomPolicy), ".");
            Log("Gameplay aspect fix loaded. FOV is preserved from the game's settings.");
            if ((g_config.dialogueZoomPolicy != DialogueZoomPolicy::Native || g_config.hotkeysEnabled) && !InstallDialogueBoundary())
                Log("Dialogue policy remains native because the validated boundary was not installed.");
            else if (g_config.dialogueZoomPolicy == DialogueZoomPolicy::Native)
                Log("Dialogue policy is Native; dialogue boundary hook bypassed.");
            if (CinematicAspectOverrideEnabled() || g_config.hotkeysEnabled) {
                std::uint8_t* cinematicAspectStore = nullptr;
                if (!ResolveCinematicAspectStore(cinematicAspectStore) ||
                    !InstallCinematicAspect(cinematicAspectStore))
                    throw std::runtime_error("cinematic aspect store validation/hook failed");
                Log("Global cinematic aspect store hooked: policy=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy), ".");
            } else {
                Log("Cinematic aspect policy is Native; aspect store hook bypassed.");
            }
            if (CinematicAspectOverrideEnabled() || g_config.hotkeysEnabled) {
                std::uint8_t* cinematicEnter = nullptr;
                std::uint8_t* cinematicExit = nullptr;
                if (!ResolveCinematicFovCallsites(cinematicEnter, cinematicExit))
                    throw std::runtime_error("cinematic live FOV signatures did not resolve uniquely");
                g_cinematicEnterHook = safetyhook::create_mid(cinematicEnter, TraceCinematicEnter);
                g_cinematicExitHook = safetyhook::create_mid(cinematicExit, TraceCinematicExit);
                if (!g_cinematicEnterHook || !g_cinematicExitHook)
                    throw std::runtime_error("cinematic coordinator hook creation failed");
            }
            if (g_config.gameplayEnabled || CinematicAspectOverrideEnabled()) {
                if (!VerifyExecutableAndInstruction())
                    throw std::runtime_error("camera-writer signature or validated FOV instruction did not match");
                Log(g_config.gameplayEnabled
                    ? "Installing validated gameplay hook."
                    : "Installing read-only camera observer for cinematic FOV aspect.");
                g_hook = safetyhook::create_mid(g_fovWriteAddress, ReplayManualTransition);
                if (!g_hook) throw std::runtime_error("validated gameplay hook creation failed");
                Log(g_config.gameplayEnabled
                    ? "Validated gameplay hook installed: true."
                    : "Read-only camera observer installed: true.");
            } else {
                Log("Gameplay aspect fix disabled by configuration.");
            }
            Log("Initialization summary: GameplayHook=", g_hook ? "PASS" : "NOT_INSTALLED",
                " CinematicAspect=", (g_cinematicAspectPatched || !CinematicAspectOverrideEnabled()) ? "PASS" : "BYPASSED",
                " CinematicFOV=", (g_cinematicEnterHook && g_cinematicExitHook) ? "PASS" : "BYPASSED",
                " Dialogue=", (g_dialogueBoundaryHook || g_config.dialogueZoomPolicy == DialogueZoomPolicy::Native)
                    ? "PASS" : "BYPASSED",
                " RuntimeTelemetry=", g_hook ? "PASS" : "NOT_INSTALLED", ".");
            if (g_config.hotkeysEnabled) {
                const auto hotkeyThread = CreateThread(nullptr, 0,
                    [](void*) -> DWORD { HotkeyLoop(); return 0; }, nullptr, 0, nullptr);
                if (!hotkeyThread) throw std::runtime_error("hotkey thread could not start");
                CloseHandle(hotkeyThread);
                Log("Hotkeys enabled: F10=Dialogue cycle, F9=Cinematics cycle for next cinematic.");
            } else {
                Log("Hotkeys disabled by configuration.");
            }
#ifdef GAMEPLAY_ONE_SHOT_CINEMATIC_TRIGGER
            const auto triggerThread = CreateThread(nullptr, 0, OneShotTriggerLoop, nullptr, 0, nullptr);
            if (!triggerThread) throw std::runtime_error("one-shot trigger thread could not start");
            CloseHandle(triggerThread);
            Log("Diagnostic one-shot trigger enabled: press F7 to arm; no timer or repeated flag write is used.");
#ifdef COMBINED_GAMEPLAY_DIAGNOSTIC
            const auto resolutionThread = CreateThread(nullptr, 0, ResolutionMonitorLoop, nullptr, 0, nullptr);
            if (!resolutionThread) throw std::runtime_error("resolution monitor thread could not start");
            CloseHandle(resolutionThread);
            Log("Combined diagnostic resolution monitor enabled: display/window/client changes are logged automatically.");
#endif
#endif
        } catch (const std::exception& exception) {
            g_dialogueBoundaryHook.reset();
            g_cinematicExitHook.reset();
            g_cinematicEnterHook.reset();
            RestoreCinematicAspect();
            Log("Gameplay hook setup failed safely: ", exception.what());
        } catch (...) {
            Log("Gameplay hook setup failed safely with an unknown exception.");
        }
        return 0;
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_DETACH) {
#ifdef POST_EXIT_CAMERA_FOV_WRITE_OWNER_TRACE
        g_postExitWriteOwnerHook.reset();
#endif
#ifdef POST_EXIT_FOV_PRODUCER_STORE_TRACE
        g_postExitProducerStoreHook.reset();
#endif
#ifdef POST_EXIT_FOV_STATE_CONSUMER_TRACE
        g_postExitConsumerHook.reset();
#endif
        g_dialogueBoundaryHook.reset();
        RestoreCinematicAspect();
        return TRUE;
    }
    if (reason != DLL_PROCESS_ATTACH) return TRUE;
    g_module = module;
    DisableThreadLibraryCalls(module);
    const auto thread = CreateThread(nullptr, 0, Initialize, nullptr, 0, nullptr);
    if (thread) CloseHandle(thread);
    return TRUE;
}
