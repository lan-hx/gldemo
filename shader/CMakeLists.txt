# cmake file by lan. all rights reserved.
cmake_minimum_required(VERSION 3.15)

FILE(GLOB_RECURSE SHADER_SOURCES ${PROJECT_SOURCE_DIR}/shader/src/*)

add_executable(shader_converter ${PROJECT_SOURCE_DIR}/shader/tools/shader_converter.cpp)
add_library(shader STATIC shader.c)
add_dependencies(shader shader_converter)

foreach (shader_source ${SHADER_SOURCES})
    get_filename_component(shader_filename ${shader_source} NAME)
    get_filename_component(shader_ext ${shader_source} EXT)
    string(REPLACE "." "" shader_type ${shader_ext})
    string(REPLACE ${shader_ext} "" shader_name ${shader_filename})
    MESSAGE(STATUS "Found shader: ${shader_name} with type ${shader_type}")

    add_custom_command(
            TARGET shader PRE_BUILD
            COMMAND $<TARGET_FILE:shader_converter> ${shader_name}_${shader_type} ${shader_source}
            ${PROJECT_SOURCE_DIR}/shader/include/${shader_name}_${shader_type}.h
            BYPRODUCTS ${PROJECT_SOURCE_DIR}/shader/include/${shader_name}_${shader_type}.h
            DEPENDS ${shader_source}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach ()