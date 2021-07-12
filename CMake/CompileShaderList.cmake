# CompileShaderList.cmake
#
# Compile a list of shaders using glslc.
#

MACRO(COMPILE_SHADER_LIST _input_list _output_list)
    FOREACH(_input ${_input_list})
        GET_FILENAME_COMPONENT(_input_path ${_input} DIRECTORY)
        GET_FILENAME_COMPONENT(_input_name ${_input} NAME_WLE)
        GET_FILENAME_COMPONENT(_input_type ${_input_name} LAST_EXT)
        STRING(SUBSTRING ${_input_type} 1 -1 _input_type) # Strip leading .

        IF(_input_type STREQUAL "inc")
            CONTINUE()
        ENDIF()
        
        # Replace leading source dir in path with binary dir
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output_path
            ${_input_path}
        )

        FILE(MAKE_DIRECTORY ${_output_path})

        SET(_output "${_output_path}/${_input_name}.spv")
        SET(_depfile "${_output_path}/${_input_name}.d")

        SET(_flags -fshader-stage=${_input_type})
        
        FOREACH(_asset_path ${ASSET_PATH})
            SET(_flags ${_flags} -I${_asset_path}/Shader)
        ENDFOREACH()

        ADD_CUSTOM_COMMAND(
            OUTPUT ${_output}
            DEPFILE ${_depfile}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE}
                ${_flags}
                -MD -MF ${_depfile}
                -o ${_output}
                ${_input}
        )

        LIST(APPEND ${_output_list} ${_output})
    ENDFOREACH()
ENDMACRO()