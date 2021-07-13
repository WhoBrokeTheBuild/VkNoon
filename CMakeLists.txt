CMAKE_MINIMUM_REQUIRED(VERSION 3.19 FATAL_ERROR)

# Allow for custom CMake modules
LIST(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

###
### Project
###

# Allow PROJECT() to manage versions
IF(POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF()

PROJECT(Noon)

INCLUDE(ProjectVersion)

###
### Third Party Dependencies
###

ADD_SUBDIRECTORY(ThirdParty)

FIND_PACKAGE(glm CONFIG REQUIRED)

IF(NOT TARGET glm::glm)
    MESSAGE(SEND_ERROR "Incompatible version of glm found")
ENDIF()

FIND_PACKAGE(fmt CONFIG REQUIRED)

FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)

FIND_PACKAGE(Python3 COMPONENTS Interpreter REQUIRED)

FIND_PACKAGE(Vulkan COMPONENTS glslc REQUIRED)

###
### Globals
###

INCLUDE(GetRuntimeDir)

GET_RUNTIME_DIR(SDL2::SDL2 _SDL2_runtime_dir)

SET(RUNTIME_PATH
    ${_SDL2_runtime_dir}
)

SET(ASSET_PATH
)

###
### Engine
###

ADD_SUBDIRECTORY(Engine)

###
### Demos
###

ADD_SUBDIRECTORY(Demos)