#ifndef VKNOON_PLATFORM_HPP
#define VKNOON_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define VKNOON_PLATFORM_WINDOWS

#elif defined(__unix__)

    #if defined(__linux__)
        // Linux
        #define VKNOON_PLATFORM_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define VKNOON_COMPILER_MSVC _MSC_VER

#elif defined(__clang__)

    // Apple Clang Compiler
    #define VKNOON_COMPILER_CLANG __clang__

#elif defined(__GNUC__) || defined(__GNUG__)
    
    // GNU Compiler Collection
    #define VKNOON_COMPILER_GCC __GNUC__

#elif defined(__ICC) || defined(__INTEL_COMPILER)

    #define VKNOON_COMPILER_ICC __INTEL_COMPILER

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define VKNOON_BUILD_RELEASE

#else

    #define VKNOON_BUILD_DEBUG

#endif

#endif // VKNOON_PLATFORM_HPP