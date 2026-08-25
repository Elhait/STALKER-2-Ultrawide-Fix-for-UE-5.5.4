#include "stdafx.h"

namespace Memory
{
    template<typename T>
    void Write(std::uint8_t* writeAddress, T value)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(writeAddress), sizeof(T), PAGE_EXECUTE_WRITECOPY, &oldProtect);
        *(reinterpret_cast<T*>(writeAddress)) = value;
        VirtualProtect((LPVOID)(writeAddress), sizeof(T), oldProtect, &oldProtect);
    }

    void PatchBytes(std::uint8_t* address, const char* pattern, unsigned int numBytes)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)address, numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((LPVOID)address, pattern, numBytes);
        VirtualProtect((LPVOID)address, numBytes, oldProtect, &oldProtect);
    }

    std::vector<int> pattern_to_byte(const char* pattern)
    {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    }

    template <typename Callback>
    void ForEachExecutableSection(void* module, Callback&& callback)
    {
        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

        auto section = IMAGE_FIRST_SECTION(ntHeaders);
        for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++section) {
            if ((section->Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
                continue;

            auto* sectionStart = reinterpret_cast<std::uint8_t*>(module) + section->VirtualAddress;
            auto sectionSize = static_cast<std::size_t>(section->Misc.VirtualSize);
            if (sectionSize != 0)
                callback(sectionStart, sectionSize);
        }
    }

    std::uint8_t* PatternScan(void* module, const char* signature)
    {
        auto patternBytes = pattern_to_byte(signature);
        std::uint8_t* result = nullptr;

        ForEachExecutableSection(module, [&](std::uint8_t* scanBytes, std::size_t scanSize) {
            if (result || patternBytes.size() > scanSize)
                return;

            for (std::size_t i = 0; i <= scanSize - patternBytes.size(); ++i) {
                bool found = true;
                for (std::size_t j = 0; j < patternBytes.size(); ++j) {
                    if (scanBytes[i + j] != patternBytes[j] && patternBytes[j] != -1) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    result = &scanBytes[i];
                    return;
                }
            }
        });

        return result;
    }

    std::vector<std::uint8_t*> PatternScanAll(void* module, const char* signature)
    {
        auto patternBytes = pattern_to_byte(signature);
        std::vector<std::uint8_t*> results;

        ForEachExecutableSection(module, [&](std::uint8_t* scanBytes, std::size_t scanSize) {
            if (patternBytes.size() > scanSize)
                return;

            for (std::size_t i = 0; i <= scanSize - patternBytes.size(); ++i) {
                bool found = true;
                for (std::size_t j = 0; j < patternBytes.size(); ++j) {
                    if (scanBytes[i + j] != patternBytes[j] && patternBytes[j] != -1) {
                        found = false;
                        break;
                    }
                }
                if (found)
                    results.push_back(&scanBytes[i]);
            }
        });

        return results;
    }

    std::uint8_t* MultiPatternScan(void* module, const std::vector<const char*>& signatures) 
    {
        for (const auto& signature : signatures) {
            if (std::uint8_t* result = PatternScan(module, signature)) {
                return result;
            }
        }
        return nullptr;
    }

    static HMODULE GetThisDllHandle()
    {
        MEMORY_BASIC_INFORMATION info;
        size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
        assert(len == sizeof(info));
        return len ? (HMODULE)info.AllocationBase : NULL;
    }

    std::uint32_t ModuleTimestamp(void* module)
    {
        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);
        return ntHeaders->FileHeader.TimeDateStamp;
    }

    std::uint8_t* GetAbsolute(std::uint8_t* address) noexcept
    {
        if (address == nullptr)
            return nullptr;

        std::int32_t offset = *reinterpret_cast<std::int32_t*>(address);
        std::uint8_t* absoluteAddress = address + 4 + offset;

        return absoluteAddress;
    }
}

namespace Util
{
    std::pair<int, int> GetPhysicalDesktopDimensions() 
    {
        if (DEVMODE devMode{ .dmSize = sizeof(DEVMODE) }; EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode))
            return { devMode.dmPelsWidth, devMode.dmPelsHeight };

        return {};
    }

    std::string wstring_to_string(const wchar_t* wstr)
    {
        size_t len = std::wcslen(wstr);
        std::string str(len, '\0');
        size_t converted = 0;
        wcstombs_s(&converted, &str[0], str.size() + 1, wstr, str.size());
        return str;
    }

    bool stringcmp_caseless(const std::string& str1, const std::string& str2) 
    {
        if (str1.size() != str2.size()) {
            return false;
        }
        return std::equal(str1.begin(), str1.end(), str2.begin(),
            [](char a, char b) {
                return std::tolower(a) == std::tolower(b);
            });
    }
}
