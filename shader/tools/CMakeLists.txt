# cmake file by lan. all rights reserved.
cmake_minimum_required(VERSION 3.15)

project(shader)

FILE(GLOB_RECURSE SHADER_SOURCES ${PROJECT_SOURCE_DIR}/../src/*)

message("shader tool dir: ${PROJECT_SOURCE_DIR}")

add_executable(shader_converter shader_converter.cpp)

foreach (shader_source ${SHADER_SOURCES})
    get_filename_component(shader_filename ${shader_source} NAME)
    get_filename_component(shader_ext ${shader_source} EXT)
    string(REPLACE "." "" shader_type ${shader_ext})
    string(REPLACE ${shader_ext} "" shader_name ${shader_filename})
    MESSAGE(STATUS "Found shader: ${shader_name} with type ${shader_type}")

    add_custom_command(
            TARGET shader_converter POST_BUILD
            COMMAND $<TARGET_FILE:shader_converter> ${shader_name}_${shader_type} ${shader_source}
            ${PROJECT_SOURCE_DIR}/../include/${shader_name}_${shader_type}.h
            BYPRODUCTS ${PROJECT_SOURCE_DIR}/../include/${shader_name}_${shader_type}.h
            DEPENDS ${shader_source}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach ()