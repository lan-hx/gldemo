# cmake file by lan. all rights reserved.
cmake_minimum_required(VERSION 3.15)

add_library(shader STATIC shader.c)

add_custom_command(
        TARGET shader PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/build/shader_build
        COMMAND ${CMAKE_COMMAND} -B ${PROJECT_SOURCE_DIR}/build/shader_build ${PROJECT_SOURCE_DIR}/shader/tools
        COMMAND ${CMAKE_COMMAND} --build ${PROJECT_SOURCE_DIR}/build/shader_build --target shader_converter
        BYPRODUCTS ${PROJECT_CURRENT_DIR}/../include/${shader_name}_${shader_type}.h
        DEPENDS ${shader_source}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
