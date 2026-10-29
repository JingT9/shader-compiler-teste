#!/usr/bin/env python3
"""Prepare CMake/header exports; never modify the Zuyu shader algorithms."""
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
src = root / "src/shader_recompiler"
if not (src / "CMakeLists.txt").is_file():
    sys.exit("ERRO: falta src/shader_recompiler/CMakeLists.txt")

include = root / "include"
include.mkdir(exist_ok=True)
for name in ("shader_compiler", "shader_recompiler"):
    target = include / name
    if target.is_symlink():
        if target.resolve() != src.resolve():
            sys.exit(f"ERRO: {target} aponta para outro diretório")
    elif target.exists():
        sys.exit(f"ERRO: {target} existe e nao e link simbolico; nao sobrescrevendo")
    else:
        target.symlink_to("../src/shader_recompiler", target_is_directory=True)

cmake = root / "CMakeLists.txt"
if not cmake.exists():
    cmake.write_text('''cmake_minimum_required(VERSION 3.20)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(shader_compiler_teste LANGUAGES CXX)
endif()
if(NOT COMMAND create_target_directory_groups)
    function(create_target_directory_groups target)
    endfunction()
endif()
add_subdirectory(src/shader_recompiler)
''')
elif "add_subdirectory(src/shader_recompiler)" not in cmake.read_text():
    sys.exit("ERRO: CMakeLists.txt ja existe com outro conteudo; nao sobrescrevendo")
print("OK: CMake e os dois aliases de headers estao preparados")
