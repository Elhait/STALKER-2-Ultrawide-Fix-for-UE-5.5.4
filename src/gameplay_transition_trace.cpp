#include "stdafx.h"

#include <safetyhook.hpp>
#include <Zydis.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <atomic>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>

namespace
{
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
    constexpr std::size_t kWriterOffset = 25;
    constexpr std::size_t kPreTraceOffset = kWriterOffset + 18;
    constexpr std::size_t kPostTraceOffset = kWriterOffset + 0x10B;

    HMODULE g_module{};
    HMODULE g_executable = GetModuleHandle(nullptr);
    std::shared_ptr<spdlog::logger> g_logger;
    SafetyHookMid g_preHook;
    SafetyHookMid g_postHook;
    std::atomic<std::uint64_t> g_nextSequence{0};
    thread_local std::uint64_t g_currentSequence{};

    struct MarkerBinding
    {
        int key;
        const char* name;
    };

    constexpr MarkerBinding kMarkerBindings[] = {
        { VK_F8, "cutscene-exit" },
        { VK_F9, "ads-enter" },
        { VK_F10, "ads-exit" },
        { VK_F11, "pause-open" },
        { VK_F12, "pause-close" },
    };

    template <typename T>
    bool SafeRead(std::uintptr_t address, T& value)
    {
        MEMORY_BASIC_INFORMATION info{};
        if (!address || !VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
        const auto end = reinterpret_cast<std::uintptr_t>(info.BaseAddress) + info.RegionSize;
        if (address > end || sizeof(value) > end - address) return false;
        std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(value));
        return true;
    }

    void LogState(const char* phase, std::uint64_t sequence, SafetyHookContext& context)
    {
        float inputFov = 0.0f;
        float secondaryFov = 0.0f;
        float aspect = 0.0f;
        float state248 = 0.0f;
        float scale25c = 0.0f;
        std::uint8_t flags = 0;
        std::uint8_t mode260 = 0;
        std::uint8_t mode261 = 0;
        std::uint8_t mode262 = 0;
        std::uint8_t mode263 = 0;
        float outputFov = 0.0f;
        float outputAspect = 0.0f;

        const auto source = static_cast<std::uintptr_t>(context.rsi);
        const auto output = static_cast<std::uintptr_t>(context.rbx);
        const bool complete =
            SafeRead(source + 0x230, inputFov) && SafeRead(source + 0x234, secondaryFov) &&
            SafeRead(source + 0x248, state248) && SafeRead(source + 0x254, aspect) &&
            SafeRead(source + 0x259, flags) && SafeRead(source + 0x25C, scale25c) &&
            SafeRead(source + 0x260, mode260) && SafeRead(source + 0x261, mode261) &&
            SafeRead(source + 0x262, mode262) && SafeRead(source + 0x263, mode263) &&
            SafeRead(output + 0x30, outputFov) && SafeRead(output + 0x5C, outputAspect);
        if (!complete) {
            if (g_logger) g_logger->warn("TRACE seq={} phase={} state-read-refused", sequence, phase);
            return;
        }

        if (g_logger) {
            g_logger->info(
                "TRACE seq={} phase={} inputFov={} secondaryFov={} state248={} aspect={} flags=0x{:02X} 25C={} 260=0x{:02X} 261=0x{:02X} 262=0x{:02X} 263=0x{:02X} outputFov={} outputAspect={}",
                sequence, phase, inputFov, secondaryFov, state248, aspect, flags, scale25c,
                mode260, mode261, mode262, mode263, outputFov, outputAspect);
        }
    }

    void TraceBeforeAdjustment(SafetyHookContext& context)
    {
        g_currentSequence = g_nextSequence.fetch_add(1, std::memory_order_relaxed) + 1;
        LogState("pre-481A", g_currentSequence, context);
    }

    void TraceAfterAdjustment(SafetyHookContext& context)
    {
        LogState("post-481A", g_currentSequence, context);
    }

    DWORD WINAPI PollManualMarkers(void*)
    {
        bool wasDown[std::size(kMarkerBindings)]{};
        for (;;) {
            for (std::size_t i = 0; i < std::size(kMarkerBindings); ++i) {
                const bool isDown = (GetAsyncKeyState(kMarkerBindings[i].key) & 0x8000) != 0;
                if (isDown && !wasDown[i] && g_logger) {
                    g_logger->info(
                        "MARKER name={} lastSequence={}",
                        kMarkerBindings[i].name,
                        g_nextSequence.load(std::memory_order_relaxed));
                }
                wasDown[i] = isDown;
            }
            Sleep(10);
        }
    }

    bool FindUniqueMatch(const std::uint8_t* start, std::size_t size, std::uint8_t*& match)
    {
        std::size_t matches = 0;
        for (std::size_t offset = 0; offset <= size - sizeof(kCameraWriterSignature); ++offset) {
            const auto* candidate = start + offset;
            bool equal = true;
            for (std::size_t i = 0; i < sizeof(kCameraWriterSignature); ++i) {
                if ((i < 17 || i >= 21) && candidate[i] != kCameraWriterSignature[i]) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                match = const_cast<std::uint8_t*>(candidate);
                ++matches;
            }
        }
        return matches == 1;
    }

    bool ResolveTraceSites(std::uint8_t*& preSite, std::uint8_t*& postSite)
    {
        const auto* base = reinterpret_cast<const std::uint8_t*>(g_executable);
        const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
        const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE) return false;

        const auto* section = IMAGE_FIRST_SECTION(nt);
        for (std::uint16_t i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
            if (std::memcmp(section->Name, ".text", 5) != 0) continue;
            const auto* start = base + section->VirtualAddress;
            const auto size = static_cast<std::size_t>(section->Misc.VirtualSize);
            if (size < sizeof(kCameraWriterSignature) || size <= kPostTraceOffset + 7) return false;

            std::uint8_t* match = nullptr;
            if (!FindUniqueMatch(start, size, match)) return false;
            preSite = match + kPreTraceOffset;
            postSite = match + kPostTraceOffset;

            constexpr std::uint8_t preBytes[] = { 0x0F, 0xB6, 0x96, 0x59, 0x02, 0x00, 0x00 };
            constexpr std::uint8_t postBytes[] = { 0x80, 0xBE, 0x40, 0x02, 0x00, 0x00, 0x00 };
            return std::memcmp(preSite, preBytes, sizeof(preBytes)) == 0 &&
                std::memcmp(postSite, postBytes, sizeof(postBytes)) == 0;
        }
        return false;
    }

    DWORD WINAPI Initialize(void*)
    {
        WCHAR modulePath[MAX_PATH]{};
        GetModuleFileNameW(g_module, modulePath, MAX_PATH);
        const auto logPath = std::filesystem::path(modulePath).remove_filename() / "STALKER2GameplayTransitionTrace.log";
        std::ofstream(logPath, std::ios::out | std::ios::trunc).close();
        try {
            g_logger = spdlog::basic_logger_mt("STALKER2GameplayTransitionTrace", logPath.string(), true);
            g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
            g_logger->flush_on(spdlog::level::info);
        } catch (...) { return 0; }

        std::uint8_t* preSite = nullptr;
        std::uint8_t* postSite = nullptr;
        if (!ResolveTraceSites(preSite, postSite)) {
            g_logger->error("TRACE setup refused: unique gameplay writer or validated trace sites not found.");
            return 0;
        }

        try {
            g_preHook = safetyhook::create_mid(preSite, TraceBeforeAdjustment);
            if (!g_preHook) throw std::runtime_error("pre hook was not created");
            g_postHook = safetyhook::create_mid(postSite, TraceAfterAdjustment);
            if (!g_postHook) throw std::runtime_error("post hook was not created");
            g_logger->info("TRACE hooks installed: pre-481A and post-481A.");
            const auto markerThread = CreateThread(nullptr, 0, PollManualMarkers, nullptr, 0, nullptr);
            if (!markerThread) throw std::runtime_error("marker thread was not created");
            CloseHandle(markerThread);
            g_logger->info("TRACE manual markers: F8=cutscene-exit F9=ads-enter F10=ads-exit F11=pause-open F12=pause-close.");
        } catch (...) {
            g_postHook.reset();
            g_preHook.reset();
            g_logger->error("TRACE setup refused safely; partial hooks rolled back.");
        }
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
