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
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
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
    constexpr float kCinemaAspect = 21.0f / 9.0f;
    constexpr float kNativeAspect = 16.0f / 9.0f;
    constexpr std::uint8_t kCinematicStorePrefix[] = { 0xC7, 0x80, 0x54, 0x02, 0x00, 0x00 };
    constexpr std::uint8_t kCinematicOriginalImmediate[] = { 0x39, 0x8E, 0xE3, 0x3F };
    constexpr std::size_t kCinematicImmediateOffset = 6;
    constexpr std::size_t kCinematicStoreInstructionLength = 10;
    constexpr float kRecoveryEpsilon = 0.01f;

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

    struct FeatureConfig
    {
        bool gameplayEnabled{true};
        CinematicAspectPolicy cinematicAspectPolicy{CinematicAspectPolicy::Auto};
        bool cinematicAspectPolicyExplicit{};
        bool cinematicFovFix{true};
    };

    HMODULE g_module{};
    HMODULE g_executable = GetModuleHandle(nullptr);
    std::uint8_t* g_fovWriteAddress{};
    SafetyHookMid g_hook;
    std::shared_ptr<spdlog::logger> g_logger;
    FeatureConfig g_config{};
    std::atomic<ReplayState> g_state{ReplayState::WaitingForAutomaticUpdate};
    std::atomic<std::uint32_t> g_lastCameraMode{};
    std::atomic<std::uint64_t> g_transitionTraceSequence{0};
    std::atomic<CoordinatorState> g_coordinator{CoordinatorState::Gameplay};
    std::atomic<bool> g_cinematicFovApplied{false};
    std::atomic<float> g_exitTargetFov{0.0f};
    SafetyHookMid g_cinematicEnterHook;
    SafetyHookMid g_cinematicExitHook;
    std::uint8_t* g_cinematicAspectStore{};
    std::uint8_t g_cinematicOriginalImmediate[sizeof(kCinematicOriginalImmediate)]{};
    bool g_cinematicAspectPatched{};
    SafetyHookMid g_cinematicAspectStoreHook;
    std::atomic<float> g_lastObservedAspect{0.0f};
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

    const char* ReplayStateName(ReplayState state)
    {
        switch (state) {
        case ReplayState::WaitingForAutomaticUpdate: return "WaitingForAutomaticUpdate";
        case ReplayState::AppliedConstrainPass: return "AppliedConstrainPass";
        case ReplayState::Complete: return "Complete";
        }
        return "Unknown";
    }

    bool IsUltrawideAspect(float aspect)
    {
        return std::isfinite(aspect) && aspect > kNativeAspect + 0.001f;
    }

    template <typename... Args>
    void Log(Args&&... args)
    {
        if (!g_logger) return;
        std::ostringstream message;
        (message << ... << args);
        g_logger->info("{}", message.str());
    }

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

    bool CinematicAspectOverrideEnabled()
    {
        return g_config.cinematicAspectPolicy != CinematicAspectPolicy::Native;
    }

    bool LoadFeatureConfig(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path)) {
            std::ofstream created(path, std::ios::out | std::ios::trunc);
            if (!created) return false;
            created << "; STALKER 2 Ultrawide Fix configuration\n"
                << "; Changes apply after restarting the game.\n"
                << "\n[Gameplay]\n"
                << "; Correct gameplay aspect behavior on ultrawide displays.\n"
                << "Enabled=true\n"
                << "\n[Cinematics]\n"
                << "; Auto, Native, 16:9, 21:9, 32:9\n"
                << "AspectRatio=Auto\n"
                << "; Apply Hor+ FOV correction for the selected aspect.\n"
                << "FovCorrection=true\n";
            return static_cast<bool>(created);
        }

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
            bool value = true;
            if (!ParseBool(line.substr(separator + 1), value)) continue;
            if (section == "Gameplay" && key == "Enabled") g_config.gameplayEnabled = value;
            else if (section == "Cinematics" && key == "FovCorrection") g_config.cinematicFovFix = value;
            else if (section == "Cinematics" && key == "AspectFix" && !g_config.cinematicAspectPolicyExplicit)
                g_config.cinematicAspectPolicy = value ? CinematicAspectPolicy::Auto : CinematicAspectPolicy::Native;
            else if (section == "Cinematics" && key == "FovFix") g_config.cinematicFovFix = value;
            else if (section == "Features" && key == "GameplayAspectFix") g_config.gameplayEnabled = value;
            else if (section == "Features" && key == "CinematicAspectFix" && !g_config.cinematicAspectPolicyExplicit)
                g_config.cinematicAspectPolicy = value ? CinematicAspectPolicy::Auto : CinematicAspectPolicy::Native;
            else if (section == "Features" && key == "CinematicFovFix") g_config.cinematicFovFix = value;
        }
        return true;
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
        if (object && SafeRead(object + kAspectOffset, aspect) && IsUltrawideAspect(aspect))
            return aspect;
        return g_lastObservedAspect.load(std::memory_order_acquire);
    }

    float ResolveCinematicAspect(std::uintptr_t object)
    {
        switch (g_config.cinematicAspectPolicy) {
        case CinematicAspectPolicy::Forced16x9: return kNativeAspect;
        case CinematicAspectPolicy::Forced21x9: return kCinemaAspect;
        case CinematicAspectPolicy::Forced32x9: return kWideAspect;
        case CinematicAspectPolicy::Auto:
        case CinematicAspectPolicy::Native:
            return ReadRuntimeAspect(object);
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

    void ApplyCinematicAspectStore(SafetyHookContext& context)
    {
        const auto targetObject = static_cast<std::uintptr_t>(context.rax);
        const float observedAspect = ReadRuntimeAspect(targetObject);
        const float aspect = ResolveCinematicAspect(targetObject);
        const bool writable = targetObject &&
            targetObject <= (std::numeric_limits<std::uintptr_t>::max)() - kAspectOffset &&
            IsWritable(targetObject + kAspectOffset, sizeof(aspect));
        if (writable) {
            std::memcpy(reinterpret_cast<void*>(targetObject + kAspectOffset), &aspect, sizeof(aspect));
            Log("Cinematic aspect store: object=0x", std::hex, targetObject, std::dec,
                " aspect=", aspect, " policy=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy),
                " source=", g_config.cinematicAspectPolicy == CinematicAspectPolicy::Auto
                    ? (IsUltrawideAspect(observedAspect) ? "runtime-camera" : "native-fallback")
                    : "configured", ".");
        } else {
            Log("Cinematic aspect store refused: target object was not writable; native store skipped.");
        }
        // The hook replaces C7 80 [disp32] [imm32]. The original store has no
        // control-flow side effects, so resuming after its ten-byte encoding
        // preserves the native lifecycle and following instructions.
        context.rip += kCinematicStoreInstructionLength;
    }

    bool LogCameraModeChange(std::uintptr_t source, std::uintptr_t output, float primaryFov,
        float aspect, std::uint8_t flags)
    {
        float secondaryFov = 0.0f;
        float outputFov = 0.0f;
        float outputAspect = 0.0f;
        float rawSourceAspect = 0.0f;
        std::uint8_t rawSourceFlags = 0;
        std::uint8_t selector = 0;
        SafeRead(source + 0x234, secondaryFov);
        SafeRead(source + kAspectOffset, rawSourceAspect);
        SafeRead(source + kFlagsOffset, rawSourceFlags);
        SafeRead(source + 0x262, selector);
        SafeRead(output + 0x30, outputFov);
        SafeRead(output + 0x5C, outputAspect);

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
            " primaryFOV=", primaryFov, " secondaryFOV=", secondaryFov,
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
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) || !SafeRead(source + kFlagsOffset, flags)) return;
        if (IsUltrawideAspect(aspect))
            g_lastObservedAspect.store(aspect, std::memory_order_release);
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
            if (WriteAspectAndFlags(source, aspect, 0x5)) {
                g_state.store(ReplayState::AppliedConstrainPass, std::memory_order_relaxed);
                Log("Replayed constrained pass: fov=", fov, " aspect=", aspect, " flags=0x5.");
            } else {
                Log("Replay refused: constrained-pass fields were not writable.");
            }
            logPost();
            return;
        }

        if (state == ReplayState::AppliedConstrainPass) {
            // On the following camera update, restore Auto's native aspect and flags.
            if (WriteAspectAndFlags(source, kNativeAspect, 0x4)) {
                g_state.store(ReplayState::Complete, std::memory_order_relaxed);
                Log("Replayed Auto restore: fov=", fov, " aspect=", kNativeAspect, " flags=0x4.");
            } else {
                Log("Replay refused: Auto-restore fields were not writable.");
            }
        }
        logPost();
    }

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
        const auto exitMatches = Memory::PatternScanAll(g_executable, kCinematicExitSignature);
        if (enterMatches.size() != 1 || exitMatches.size() != 1) {
            Log("Cinematic FOV signatures rejected: enterMatches=", enterMatches.size(),
                " exitMatches=", exitMatches.size(), ".");
            return false;
        }
        if (!ValidateEnterBoundary(enterMatches.front(), enter) ||
            !ValidateExitBoundary(exitMatches.front(), exit)) return false;
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
        if (g_config.cinematicFovFix) {
            bool expected = false;
            if (!g_cinematicFovApplied.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) return;
        }
        const float before = context.xmm0.f32[0];
        const float aspect = ResolveCinematicAspect(0);
        const float after = g_config.cinematicFovFix && std::isfinite(before) && std::isfinite(aspect) &&
            before > 1.0f && before < 179.0f && aspect > 1.0f
            ? CinematicHorPlus(before, aspect) : before;
        if (std::isfinite(after) && after > 1.0f && after < 179.0f) context.xmm0.f32[0] = after;
        g_coordinator.store(CoordinatorState::CinematicActive, std::memory_order_release);
        Log("Global cinematic ENTER: aspect=", aspect, " authoredFov=", before,
            " transformedFov=", context.xmm0.f32[0], " fovFix=", g_config.cinematicFovFix,
            " aspectPolicy=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy), ".",
            " Gameplay replay suppressed.");
    }

    void TraceCinematicExit(SafetyHookContext& context)
    {
        g_exitTargetFov.store(context.xmm0.f32[0], std::memory_order_release);
        g_cinematicFovApplied.store(false, std::memory_order_release);
        g_coordinator.store(CoordinatorState::CinematicExiting, std::memory_order_release);
        Log("Global cinematic EXIT: nativeTargetFov=", context.xmm0.f32[0],
            ". Gameplay replay suppressed until native recovery.");
    }

    void ReplayManualTransition(SafetyHookContext& context)
    {
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
                Log("Global coordinator: native recovery complete; delta=", delta,
                    " target=", targetFov, " current=", currentFov,
                    ". Same writer invocation returned without replay.");
            }
            return;
        }
        if (!g_config.gameplayEnabled) {
            float aspect = 0.0f;
            const auto source = static_cast<std::uintptr_t>(context.rsi);
            if (SafeRead(source + kAspectOffset, aspect) && IsUltrawideAspect(aspect))
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

    DWORD WINAPI Initialize(void*)
    {
        WCHAR modulePath[MAX_PATH]{};
        GetModuleFileNameW(g_module, modulePath, MAX_PATH);
        const auto moduleDirectory = std::filesystem::path(modulePath).remove_filename();
        const auto logPath = moduleDirectory / "STALKER2UltrawideFix.log";
        const auto configPath = moduleDirectory / "STALKER2UltrawideFix.ini";
        std::ofstream(logPath, std::ios::out | std::ios::trunc).close();
        try {
            g_logger = spdlog::basic_logger_mt("STALKER2UltrawideFix", logPath.string(), true);
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
            if (!LoadFeatureConfig(configPath))
                Log("Configuration unavailable; using defaults with all fixes enabled.");
            Log("Configuration: Gameplay.Enabled=", g_config.gameplayEnabled,
                " Cinematic.AspectRatio=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy),
                " Cinematic.FovCorrection=", g_config.cinematicFovFix, ".");
            Log("Gameplay aspect fix loaded. FOV is preserved from the game's settings.");
            if (CinematicAspectOverrideEnabled()) {
                std::uint8_t* cinematicAspectStore = nullptr;
                if (!ResolveCinematicAspectStore(cinematicAspectStore) ||
                    !InstallCinematicAspect(cinematicAspectStore))
                    throw std::runtime_error("cinematic aspect store validation/hook failed");
                Log("Global cinematic aspect store hooked: policy=", CinematicAspectPolicyName(g_config.cinematicAspectPolicy), ".");
            } else {
                Log("Cinematic aspect policy is Native; aspect store hook bypassed.");
            }
            if (CinematicAspectOverrideEnabled() || g_config.cinematicFovFix) {
                std::uint8_t* cinematicEnter = nullptr;
                std::uint8_t* cinematicExit = nullptr;
                if (!ResolveCinematicFovCallsites(cinematicEnter, cinematicExit))
                    throw std::runtime_error("cinematic live FOV signatures did not resolve uniquely");
                g_cinematicEnterHook = safetyhook::create_mid(cinematicEnter, TraceCinematicEnter);
                g_cinematicExitHook = safetyhook::create_mid(cinematicExit, TraceCinematicExit);
                if (!g_cinematicEnterHook || !g_cinematicExitHook)
                    throw std::runtime_error("cinematic coordinator hook creation failed");
            }
            if (g_config.gameplayEnabled || g_config.cinematicFovFix) {
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
