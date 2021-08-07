#ifndef NOON_MACROS_HPP
#define NOON_MACROS_HPP

#include <Noon/Platform.hpp>

#define NOON_DISALLOW_COPY_AND_ASSIGN(TypeName)         \
    TypeName(const TypeName&) = delete;                 \
    TypeName& operator=(const TypeName&) = delete;

#define NOON_STRINGIFY(x) _NOON_STRINGIFY(x)
#define _NOON_STRINGIFY(x) #x

#if defined(NOON_COMPILER_MSVC)

    #define NOON_FUNCTION_NAME() __FUNCSIG__

#elif defined(NOON_COMPILER_GCC) || defined(NOON_COMPILER_CLANG)

    #define NOON_FUNCTION_NAME() __PRETTY_FUNCTION__

#else

    #define NOON_FUNCTION_NAME() __func__

#endif

#if defined(NOON_COMPILER_MSVC)

    #define NOON_DISABLE_WARNINGS() \
        __pragma(warning(push, 0))
    
    #define NOON_ENABLE_WARNINGS() \
        __pragma(warning(pop))

#elif defined(NOON_COMPILER_CLANG)

    #define NOON_DISABLE_WARNINGS() \
        _Pragma("clang diagnostic push")                    \
        _Pragma("clang diagnostic ignored \"-Wall\"")
    
    #define NOON_ENABLE_WARNINGS() \
        _Pragma("clang diagnostic pop")

#elif defined(NOON_COMPILER_GCC)

    #define NOON_DISABLE_WARNINGS() \
        _Pragma("GCC diagnostic push")                      \
        _Pragma("GCC diagnostic ignored \"-Wall\"")
    
    #define NOON_ENABLE_WARNINGS() \
        _Pragma("GCC diagnostic pop")

#else

    #define NOON_DISABLE_WARNINGS()

    #define NOON_ENABLE_WARNINGS()

#endif

#endif // NOON_MACROS_HPP