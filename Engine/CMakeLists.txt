
SET(_target Noon)

###
### Source Files
###

FILE(
    GLOB_RECURSE
    _sources
    "Public/*.h"
    "Public/*.hpp"
    "Private/*.h"
    "Private/*.hpp"
    "Private/*.c"
    "Private/*.cpp"
)

###
### Template Files
###

INCLUDE(ConfigureFileList)

FILE(
    GLOB_RECURSE
    _sources_in
    "Public/*.in"
    "Private/*.in"
)

CONFIGURE_FILE_LIST("${_sources_in}" _sources_out)

###
### Asset Processing
###

LIST(APPEND ASSET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Asset)
SET(ASSET_PATH ${ASSET_PATH} PARENT_SCOPE)

FILE(
    GLOB_RECURSE
    _shaders_in
    "Asset/Shader/*.glsl"
)

INCLUDE(CompileShaderList)

COMPILE_SHADER_LIST("${_shaders_in}" _shaders_out)

###
### Target Configuration
###

ADD_LIBRARY(
    ${_target} SHARED
    ${_sources}
    ${_sources_out}
    ${_shaders_out}
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        fmt::fmt
        glm::glm
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
        ${CMAKE_CURRENT_BINARY_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
        ${CMAKE_CURRENT_BINARY_DIR}/Private
)

TARGET_COMPILE_DEFINITIONS(
    ${_target}
    PUBLIC
        # Disable VS "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DEFINE_SYMBOL "NOON_EXPORT"
)

TARGET_COMPILE_OPTIONS(
    ${_target}
    PUBLIC
        # Configure VS to use C++20, since it ignores CXX_STANDARD
        $<$<CXX_COMPILER_ID:MSVC>: /std:c++latest>

        # Configure exception handling model
        $<$<CXX_COMPILER_ID:MSVC>: /EHs>

        # Build with multiple processors
        $<$<CXX_COMPILER_ID:MSVC>: /MP>

        # Force windows to use UTF-8
        $<$<CXX_COMPILER_ID:MSVC>: /utf-8>

        # Enable most warnings, disable unknown pragmas warning
        $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -Wno-nullability-completeness>
        $<$<CXX_COMPILER_ID:MSVC>:  /wd4068>
)

LIST(APPEND RUNTIME_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(RUNTIME_PATH ${RUNTIME_PATH} PARENT_SCOPE)
