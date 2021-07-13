#ifndef NOON_PLATFORM_HPP
#define NOON_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define NOON_PLATFORM_WINDOWS

#elif defined(__unix__)

    #if defined(__linux__)
        // Linux
        #define NOON_PLATFORM_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define NOON_COMPILER_MSVC _MSC_VER

#elif defined(__clang__)

    // Apple Clang Compiler
    #define NOON_COMPILER_CLANG __clang__

#elif defined(__GNUC__) || defined(__GNUG__)
    
    // GNU Compiler Collection
    #define NOON_COMPILER_GCC __GNUC__

#elif defined(__ICC) || defined(__INTEL_COMPILER)

    #define NOON_COMPILER_ICC __INTEL_COMPILER

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define NOON_BUILD_RELEASE

#else

    #define NOON_BUILD_DEBUG

#endif

#endif // NOON_PLATFORM_HPP