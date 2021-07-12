
MACRO(DEFINE_DEMO _target)

    ###
    ### Source Files
    ###

    FILE(
        GLOB_RECURSE
        _sources
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
        "Private/*.in"
    )

    CONFIGURE_FILE_LIST("${_sources_in}" _sources_out)

    ###
    ### Shader Processing
    ###

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

    ADD_EXECUTABLE(
        ${_target}
        ${_sources}
        ${_sources_out}
        ${_shaders_out}
    )

    TARGET_LINK_LIBRARIES(
        ${_target}
        PRIVATE
            VkNoon
    )

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
            ${CMAKE_CURRENT_BINARY_DIR}/Source
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
    )

    STRING(REPLACE " " "\\ " ASSET_PATH "${ASSET_PATH}")
    STRING(REPLACE " " "\\ " RUNTIME_PATH "${RUNTIME_PATH}")

    IF(WIN32)
        SET(_ext ".exe")
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/Scripts/generate-launch-targets.py
            ${CMAKE_CURRENT_BINARY_DIR}/${_target}${_ext}
            ${CMAKE_CURRENT_SOURCE_DIR}
            "${ASSET_PATH}"
            "${RUNTIME_PATH}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

ENDMACRO()


ADD_SUBDIRECTORY(HelloWorld)