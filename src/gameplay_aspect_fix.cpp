#include "helper.hpp"

#include <safetyhook.hpp>
#include <Zydis.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>

namespace
{
    constexpr std::uint32_t kExpectedTimeDateStamp = 0x60AB1AA8;
    constexpr std::uint32_t kExpectedImageSize = 0x0B94D000;
    constexpr std::uintptr_t kFovWriteRva = 0x00AF3A17;
    constexpr std::uintptr_t kAspectOffset = 0x254;
    constexpr std::uintptr_t kFlagsOffset = 0x259;
    constexpr float kWideAspect = 32.0f / 9.0f;
    constexpr float kCinemaAspect = 21.0f / 9.0f;
    constexpr float kNativeAspect = 16.0f / 9.0f;

    enum class ReplayState : std::uint32_t { WaitingForAutomaticUpdate, AppliedConstrainPass, Complete };

    HMODULE g_module{};
    HMODULE g_executable = GetModuleHandle(nullptr);
    SafetyHookMid g_hook;
    std::shared_ptr<spdlog::logger> g_logger;
    std::atomic<ReplayState> g_state{ReplayState::WaitingForAutomaticUpdate};
    std::atomic<std::uint32_t> g_lastCameraMode{};

    template <typename... Args>
    void Log(Args&&... args)
    {
        if (!g_logger) return;
        std::ostringstream message;
        (message << ... << args);
        g_logger->info("{}", message.str());
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

    bool WriteAspectAndFlags(std::uintptr_t source, float aspect, std::uint8_t flags)
    {
        if (!IsWritable(source + kAspectOffset, sizeof(aspect)) || !IsWritable(source + kFlagsOffset, sizeof(flags))) return false;
        std::memcpy(reinterpret_cast<void*>(source + kAspectOffset), &aspect, sizeof(aspect));
        std::memcpy(reinterpret_cast<void*>(source + kFlagsOffset), &flags, sizeof(flags));
        return true;
    }

    void LogCameraModeChange(std::uintptr_t source, std::uintptr_t output, float primaryFov,
        float aspect, std::uint8_t flags)
    {
        float secondaryFov = 0.0f;
        float outputFov = 0.0f;
        float outputAspect = 0.0f;
        std::uint8_t selector = 0;
        SafeRead(source + 0x234, secondaryFov);
        SafeRead(source + 0x262, selector);
        SafeRead(output + 0x30, outputFov);
        SafeRead(output + 0x5C, outputAspect);

        std::uint32_t aspectBits{};
        std::memcpy(&aspectBits, &aspect, sizeof(aspectBits));
        const std::uint32_t mode = aspectBits ^ (static_cast<std::uint32_t>(flags) << 1) ^
            (static_cast<std::uint32_t>(selector) << 9);
        if (g_lastCameraMode.exchange(mode, std::memory_order_relaxed) == mode)
            return;

        Log("Camera mode: primaryFOV=", primaryFov, " secondaryFOV=", secondaryFov,
            " aspect=", aspect, " flags=0x", std::hex, static_cast<int>(flags),
            " selector=0x", static_cast<int>(selector), std::dec,
            " outputFOV(before)=", outputFov, " outputAspect(before)=", outputAspect, ".");
    }

    void ReplayManualTransition(SafetyHookContext& context)
    {
        const auto source = context.rsi;
        const float fov = context.xmm0.f32[0];
        float aspect = 0.0f;
        std::uint8_t flags = 0;
        if (!SafeRead(source + kAspectOffset, aspect) || !SafeRead(source + kFlagsOffset, flags)) return;
        LogCameraModeChange(source, context.rbx, fov, aspect, flags);

        if (std::fabs(aspect - kCinemaAspect) <= 0.001f && flags == 0x5) {
            // Preserve constrained aspect ratio for a real 21:9 output. The FOV source
            // is already correct; only the camera aspect state needs normalization.
            if (WriteAspectAndFlags(source, kNativeAspect, 0x5))
                Log("Normalized 21:9 camera aspect while preserving constrained aspect ratio.");
            return;
        }

        auto state = g_state.load(std::memory_order_relaxed);
        if (state == ReplayState::Complete && std::fabs(aspect - kWideAspect) <= 0.001f && flags == 0x4) {
            // Cutscenes can rebuild the gameplay camera and restore the same broken
            // Auto state seen during startup. Arm the proven two-pass transition again.
            g_state.store(ReplayState::WaitingForAutomaticUpdate, std::memory_order_relaxed);
            state = ReplayState::WaitingForAutomaticUpdate;
            Log("Gameplay camera was rebuilt; re-arming aspect transition.");
        }

        if (state == ReplayState::WaitingForAutomaticUpdate) {
            // The game has performed its own startup update. Recreate the observed
            // temporary constrained pass that occurs when the user applies 16:9.
            if (std::fabs(aspect - kWideAspect) > 0.001f || flags != 0x4) return;
            if (WriteAspectAndFlags(source, kWideAspect, 0x5)) {
                g_state.store(ReplayState::AppliedConstrainPass, std::memory_order_relaxed);
                Log("Replayed constrained pass: fov=", fov, " aspect=", kWideAspect, " flags=0x5.");
            } else {
                Log("Replay refused: constrained-pass fields were not writable.");
            }
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
    }

    bool VerifyExecutableAndInstruction()
    {
        const auto* base = reinterpret_cast<const std::uint8_t*>(g_executable);
        const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
        const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE || nt->FileHeader.TimeDateStamp != kExpectedTimeDateStamp ||
            nt->OptionalHeader.SizeOfImage != kExpectedImageSize) return false;

        ZydisDecoder decoder{};
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT_VISIBLE]{};
        auto* target = const_cast<std::uint8_t*>(base) + kFovWriteRva;
        return ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)) &&
            ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, target, 15, &instruction, operands)) &&
            instruction.mnemonic == ZYDIS_MNEMONIC_MOVSS && instruction.operand_count_visible >= 2 &&
            operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY && operands[1].type == ZYDIS_OPERAND_TYPE_REGISTER &&
            operands[0].mem.base == ZYDIS_REGISTER_RBX && operands[0].mem.disp.has_displacement &&
            operands[0].mem.disp.value == 0x30 && operands[1].reg.value == ZYDIS_REGISTER_XMM0;
    }

    DWORD WINAPI Initialize(void*)
    {
        WCHAR modulePath[MAX_PATH]{};
        GetModuleFileNameW(g_module, modulePath, MAX_PATH);
        const auto logPath = std::filesystem::path(modulePath).remove_filename() / "STALKER2GameplayAspectFix.log";
        std::ofstream(logPath, std::ios::out | std::ios::trunc).close();
        try {
            g_logger = spdlog::basic_logger_mt("STALKER2GameplayAspectFix", logPath.string(), true);
            g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
            g_logger->flush_on(spdlog::level::info);
        } catch (...) { return 0; }

        Log("Gameplay aspect fix loaded. FOV is preserved from the game's settings.");
        if (!VerifyExecutableAndInstruction()) {
            Log("Test refused: executable or validated FOV instruction did not match.");
            return 0;
        }
        g_hook = safetyhook::create_mid(reinterpret_cast<std::uint8_t*>(g_executable) + kFovWriteRva,
            ReplayManualTransition);
        Log("Validated gameplay hook installed: ", static_cast<bool>(g_hook), ".");
        return 0;
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
    if (reason != DLL_PROCESS_ATTACH) return TRUE;
    g_module = module;
    DisableThreadLibraryCalls(module);
    const auto thread = CreateThread(nullptr, 0, Initialize, nullptr, 0, nullptr);
    if (thread) CloseHandle(thread);
    return TRUE;
}
