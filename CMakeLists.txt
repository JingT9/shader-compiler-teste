cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(shader_compiler_teste LANGUAGES CXX)
endif()

if(NOT COMMAND create_target_directory_groups)
    function(create_target_directory_groups target)
    endfunction()
endif()

add_subdirectory(src/shader_recompiler)

target_include_directories(shader_recompiler PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
