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
#include <stdexcept>
#include <thread>

namespace
{
    constexpr std::uint8_t kLetterboxSignatureA[] = {
        0x48, 0x8B, 0x41, 0x18, 0x48, 0x8B, 0x88, 0xF8, 0x00, 0x00, 0x00,
        0x80, 0x89, 0x59, 0x02, 0x00, 0x00, 0x01, 0x48, 0x8B, 0x80, 0xF8,
        0x00, 0x00, 0x00, 0xC7, 0x80, 0x54, 0x02, 0x00, 0x00, 0x39, 0x8E,
        0xE3, 0x3F, 0xC3,
    };
    constexpr std::uint8_t kLetterboxSignatureB[] = {
        0x48, 0x8B, 0x41, 0x18, 0x48, 0x8B, 0x88, 0xF8, 0x00, 0x00, 0x00,
        0x80, 0x89, 0x59, 0x02, 0x00, 0x00, 0x01, 0x48, 0x8B, 0x80, 0xF8,
        0x00, 0x00, 0x00, 0xC7, 0x80, 0x54, 0x02, 0x00, 0x00, 0x39, 0x8E,
        0xE3, 0x3F, 0xB0, 0x01, 0xC3,
    };
    constexpr std::size_t kSetterOffset = 0x19;
    constexpr std::size_t kSetterLength = 10;
    constexpr std::uintptr_t kAspectOffset = 0x254;

    HMODULE g_module{};
    HMODULE g_executable = GetModuleHandle(nullptr);
    std::shared_ptr<spdlog::logger> g_logger;
    SafetyHookMid g_hookA;
    SafetyHookMid g_hookB;
    std::atomic<float> g_displayAspect{0.0f};
    std::atomic_bool g_stopAspectUpdates{false};

    bool IsWritable(std::uintptr_t address, std::size_t size)
    {
        MEMORY_BASIC_INFORMATION info{};
        if (!address || !VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
        const auto regionEnd = reinterpret_cast<std::uintptr_t>(info.BaseAddress) + info.RegionSize;
        const DWORD writable = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        return address <= regionEnd && size <= regionEnd - address && (info.Protect & writable);
    }

    void Log(const char* message)
    {
        if (g_logger) g_logger->info("{}", message);
    }

    bool ReadDisplayAspect(float& aspect)
    {
        DEVMODE display{ .dmSize = sizeof(DEVMODE) };
        if (!EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &display) || display.dmPelsHeight == 0)
            return false;
        aspect = static_cast<float>(display.dmPelsWidth) / static_cast<float>(display.dmPelsHeight);
        return aspect > 0.0f;
    }

    DWORD WINAPI DisplayAspectOwner(void*)
    {
        while (!g_stopAspectUpdates.load(std::memory_order_relaxed)) {
            float aspect = 0.0f;
            if (ReadDisplayAspect(aspect)) g_displayAspect.store(aspect, std::memory_order_relaxed);
            Sleep(500);
        }
        return 0;
    }

    void ApplyDisplayAspect(SafetyHookContext& context)
    {
        // This runs after the validated original setter instruction. RAX is the
        // same target object used by that instruction; no new dereference is made.
        const auto target = static_cast<std::uintptr_t>(context.rax) + kAspectOffset;
        const float aspect = g_displayAspect.load(std::memory_order_relaxed);
        if (aspect <= 0.0f || !IsWritable(target, sizeof(aspect))) return;
        std::memcpy(reinterpret_cast<void*>(target), &aspect, sizeof(aspect));
    }

    bool FindUniqueMatch(const std::uint8_t* sectionStart, std::size_t sectionSize,
        const std::uint8_t* signature, std::size_t signatureSize, std::uint8_t*& match)
    {
        std::size_t matches = 0;
        for (std::size_t offset = 0; offset <= sectionSize - signatureSize; ++offset) {
            if (std::memcmp(sectionStart + offset, signature, signatureSize) == 0) {
                match = const_cast<std::uint8_t*>(sectionStart + offset);
                ++matches;
            }
        }
        return matches == 1;
    }

    bool DecodeSetterAndSuccessor(std::uint8_t* signatureMatch, std::size_t signatureSize,
        std::uint8_t expectedSuccessorFirstByte, std::uint8_t*& postSetter)
    {
        auto* setter = signatureMatch + kSetterOffset;
        ZydisDecoder decoder{};
        ZydisDecodedInstruction instruction{};
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
        if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)) ||
            !ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, setter, 15, &instruction, operands)) ||
            instruction.mnemonic != ZYDIS_MNEMONIC_MOV || instruction.length != kSetterLength ||
            instruction.operand_count_visible < 2 || operands[0].type != ZYDIS_OPERAND_TYPE_MEMORY ||
            operands[0].mem.base != ZYDIS_REGISTER_RAX || !operands[0].mem.disp.has_displacement ||
            operands[0].mem.disp.value != 0x254 || operands[1].type != ZYDIS_OPERAND_TYPE_IMMEDIATE ||
            operands[1].imm.value.u != 0x3FE38E39 || setter + kSetterLength >= signatureMatch + signatureSize ||
            setter[kSetterLength] != expectedSuccessorFirstByte)
            return false;

        postSetter = setter + kSetterLength;
        return true;
    }

    bool ResolveTargets(std::uint8_t*& postA, std::uint8_t*& postB)
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
            if (size < sizeof(kLetterboxSignatureA) || size < sizeof(kLetterboxSignatureB)) return false;

            std::uint8_t* matchA = nullptr;
            std::uint8_t* matchB = nullptr;
            if (!FindUniqueMatch(start, size, kLetterboxSignatureA, sizeof(kLetterboxSignatureA), matchA) ||
                !FindUniqueMatch(start, size, kLetterboxSignatureB, sizeof(kLetterboxSignatureB), matchB)) return false;
            return DecodeSetterAndSuccessor(matchA, sizeof(kLetterboxSignatureA), 0xC3, postA) &&
                DecodeSetterAndSuccessor(matchB, sizeof(kLetterboxSignatureB), 0xB0, postB);
        }
        return false;
    }

    DWORD WINAPI Initialize(void*)
    {
        WCHAR modulePath[MAX_PATH]{};
        GetModuleFileNameW(g_module, modulePath, MAX_PATH);
        const auto logPath = std::filesystem::path(modulePath).remove_filename() / "STALKER2ExperimentalLetterboxFix.log";
        std::ofstream(logPath, std::ios::out | std::ios::trunc).close();
        try {
            g_logger = spdlog::basic_logger_mt("STALKER2ExperimentalLetterboxFix", logPath.string(), true);
            g_logger->flush_on(spdlog::level::info);
        } catch (...) { return 0; }

        float initialAspect = 0.0f;
        std::uint8_t* postA = nullptr;
        std::uint8_t* postB = nullptr;
        if (!ReadDisplayAspect(initialAspect) || !ResolveTargets(postA, postB)) {
            Log("Letterbox setup refused: display aspect or both validated targets unavailable.");
            return 0;
        }
        g_displayAspect.store(initialAspect, std::memory_order_relaxed);

        const auto ownerThread = CreateThread(nullptr, 0, DisplayAspectOwner, nullptr, 0, nullptr);
        if (!ownerThread) {
            Log("Letterbox setup refused: display-aspect owner thread could not start.");
            return 0;
        }
        CloseHandle(ownerThread);
        try {
            g_hookA = safetyhook::create_mid(postA, ApplyDisplayAspect);
            if (!g_hookA) throw std::runtime_error("hook A was not created");
            g_hookB = safetyhook::create_mid(postB, ApplyDisplayAspect);
            if (!g_hookB) throw std::runtime_error("hook B was not created");
            Log("Experimental letterbox hooks A/B installed after complete resolution and decode validation.");
        } catch (...) {
            g_hookB.reset();
            g_hookA.reset();
            g_stopAspectUpdates.store(true, std::memory_order_relaxed);
            Log("Letterbox setup refused safely; any partial hook was rolled back.");
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
