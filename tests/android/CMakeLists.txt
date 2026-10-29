# SPDX-License-Identifier: GPL-2.0-or-later
cmake_minimum_required(VERSION 3.16)
project(SKYrusShaderAndroidCheck LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(RANGE_V3_TESTS OFF CACHE BOOL "" FORCE)
set(RANGE_V3_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RANGE_V3_PERF OFF CACHE BOOL "" FORCE)
set(SIRIT_TESTS OFF CACHE BOOL "" FORCE)

set(SKYRUS_TEST_DEPENDENCIES "" CACHE PATH "Folder containing boost, fmt, range and sirit")
foreach(dependency boost fmt range sirit)
    if (NOT EXISTS "${SKYRUS_TEST_DEPENDENCIES}/${dependency}/CMakeLists.txt")
        message(FATAL_ERROR "Missing dependency ${dependency}; set SKYRUS_TEST_DEPENDENCIES")
    endif()
    add_subdirectory("${SKYRUS_TEST_DEPENDENCIES}/${dependency}" "${CMAKE_CURRENT_BINARY_DIR}/${dependency}")
endforeach()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../.." "${CMAKE_CURRENT_BINARY_DIR}/shader-compiler")
