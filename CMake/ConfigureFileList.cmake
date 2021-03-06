# ConfigureFileList.cmake
#
# This will run CONFIGURE_FILE for each file in ${_input_list}, and store them in their 
# relative location under the binary directory, with the last extension stripped. The resulting
# filenames will be stored in ${_output_list}.
#
# For example:
#   ${CMAKE_CURRENT_SOURCE_DIR}/Source/Config.h.in
# will be configured and stored in 
#   ${CMAKE_BINARY_SOURCE_DIR}/Source/config.h
#

MACRO(CONFIGURE_FILE_LIST _input_list _output_list)
    FOREACH(_input ${_input_list})
        # Replace leading source dir in path with binary dir
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output
            ${_input}
        )

        # Remove last extension
        string(REGEX MATCH "^(.*)\\.[^.]*$" _output ${_output})
        set(_output ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${_input} ${_output})

        LIST(APPEND ${_output_list} ${_output})
    ENDFOREACH()
ENDMACRO()