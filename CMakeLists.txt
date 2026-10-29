cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(shader_compiler_teste LANGUAGES CXX)
endif()

if(NOT COMMAND create_target_directory_groups)
    function(create_target_directory_groups target)
    endfunction()
endif()

# Zuyu keeps a separate set of common headers. These are imported unchanged
# by scripts/get_original_zuyu_common.sh; never silently compile against a
# different project's common/bit_field.h or common/assert.h.
set(ZUYU_COMMON_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zuyu-original/src"
    CACHE PATH "Root containing the original Zuyu common/ headers")
if(NOT EXISTS "${ZUYU_COMMON_SOURCE_DIR}/common/common_types.h")
    message(FATAL_ERROR
        "Zuyu common headers are missing at ${ZUYU_COMMON_SOURCE_DIR}. "
        "Run: bash scripts/get_original_zuyu_common.sh")
endif()

# The standalone Zuyu library normally links yuzu's `common` target. SKYrus
# does not necessarily provide it. Its shader usage needs header-only common
# utilities plus the explicitly provided assert/log bridge below.
if(NOT TARGET common)
    add_library(common INTERFACE)
    target_include_directories(common INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${ZUYU_COMMON_SOURCE_DIR}")
endif()

if(NOT TARGET fmt::fmt OR NOT TARGET sirit)
    message(FATAL_ERROR
        "shader-compiler-teste requires fmt::fmt and sirit from the SKYrus "
        "parent build. Do not replace these libraries with placeholders.")
endif()

add_subdirectory(src/shader_recompiler)

target_sources(shader_recompiler PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/compat/zuyu_assert.cpp")

target_include_directories(shader_recompiler PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
