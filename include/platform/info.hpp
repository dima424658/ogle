#pragma once

#include <string_view>

#define STR(x) #x
#define MACRO_STR(s) STR(s)

namespace Platform
{
    enum class TCompiler
    {
        GCC,
        Clang,
        MSVC,
        Unknown
    };

    enum class TOperatingSystem
    {
        Linux,
        MacOS,
        Windows,
        Unknown
    };

    enum class TArchitecture
    {
        x86,
        x86_64,
        Unknown
    };

    constexpr TCompiler GetCompilerType()
    {
#if defined(__clang__)
        return TCompiler::Clang;
#elif defined(__GNUC__)
        return TCompiler::GCC;
#elif defined(_MSC_VER)
        return TCompiler::MSVC;
#else
        return TCompiler::Unknown;
#endif
    }

    constexpr TOperatingSystem GetOperatingSystem()
    {
#if defined(__APPLE__) || defined(__MACH__)
        return TOperatingSystem::MacOS;
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
        return TOperatingSystem::Windows;
#elif defined(__linux__) || defined(linux) || defined(__linux)
        return TOperatingSystem::Linux;
#endif
    }

    constexpr TArchitecture GetArchitecture()
    {
        if constexpr (GetOperatingSystem() == TOperatingSystem::Windows)
        {
#if defined(_WIN64) || defined(WIN64)
            return TArchitecture::x86_64;
#else
            return TArchitecture::x86;
#endif
        }
        else if constexpr (GetOperatingSystem() == TOperatingSystem::Linux || GetOperatingSystem() == TOperatingSystem::MacOS)
        {
#if defined(__amd64__) || defined(__x86_64__) || defined(_M_AMD64)
            return TArchitecture::x86_64;
#else
            return TArchitecture::x86;
#endif
        }
    }

    constexpr std::string_view GetCompilerName()
    {
#if defined(__clang__)
        return "Clang";
#elif defined(__GNUC__)
        return "GCC";
#elif defined(_MSC_VER)
        return "MSVC";
#else
        return "Unknown";
#endif
    }

    constexpr std::string_view GetCompilerVersion()
    {
#if defined(__clang__)
        return MACRO_STR(__clang_major__) "." MACRO_STR(__clang_minor__) "." MACRO_STR(__clang_patchlevel__);
#elif defined(__GNUC__)
        return MACRO_STR(__GNUC__) "." MACRO_STR(__GNUC_MINOR__) "." MACRO_STR(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
        return MACRO_STR(_MSC_VER);
#else
        return "Unknown";
#endif
    }

}; // namespace Platform

#undef MACRO_STR
#undef STR