# CMakePolicies.cmake
#
# Set CMake Policy decisions to silence warnings.
#

# Allow PROJECT() to manage versions
IF(POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF()

# Allow Ninja to transform DEPFILEs
IF(POLICY CMP0116)
    CMAKE_POLICY(SET CMP0116 NEW)
ENDIF()
