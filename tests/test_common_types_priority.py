#!/usr/bin/env python3
"""Regressao: Skyline/common_types.h nao pode ocultar os tipos globais do Zuyu.

Cria uma arvore minima que reproduz a ordem real de includes do Android:
  -I.../skyline -I.../shader-compiler/src -I.../zuyu-original/src
O compilador deve encontrar os typedefs originais mesmo com o header da
aplicacao sendo encontrado primeiro nas buscas genericas <common/...>.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
compiler = shutil.which("clang++") or shutil.which("g++") or shutil.which("c++")
if compiler is None:
    raise SystemExit("ERRO: compilador C++20 nao encontrado (clang++/g++)")

with tempfile.TemporaryDirectory(prefix="zuyu-types-") as td:
    base = Path(td)
    wrapper = base / "src/shader_recompiler/common/common_types.h"
    wrapper.parent.mkdir(parents=True)
    shutil.copy2(root / "src/shader_recompiler/common/common_types.h", wrapper)

    # Simula um header do host que define tipos apenas no seu namespace.
    host = base / "skyline/common/common_types.h"
    host.parent.mkdir(parents=True)
    host.write_text("#pragma once\n#include <cstdint>\nnamespace skyline { using u32 = std::uint32_t; using u8 = std::uint8_t; }\n")
    original = base / "third_party/zuyu-original/src/common/common_types.h"
    original.parent.mkdir(parents=True)
    original.write_text("#pragma once\n#include <cstdint>\nusing u8 = std::uint8_t;\nusing u32 = std::uint32_t;\n")
    public = base / "src/shader_recompiler/program_header.h"
    public.write_text('#pragma once\n#include "common/common_types.h"\nenum class OutputTopology : u32 { PointList = 1 };\nenum class PixelImap : u8 { Unused = 0 };\n')
    src = base / "test.cpp"
    src.write_text('#include "shader_recompiler/program_header.h"\nstatic_assert(sizeof(u32) == 4);\nstatic_assert(sizeof(u8) == 1);\nint main() { return 0; }\n')
    result = subprocess.run([
        compiler, "-std=c++20", "-fsyntax-only",
        "-I" + str(base / "skyline"),
        "-I" + str(base / "src"),
        "-I" + str(base / "third_party/zuyu-original/src"),
        str(src),
    ], capture_output=True, text=True)
    if result.returncode:
        raise SystemExit("ERRO: include do Skyline ocultou os tipos do Zuyu:\n" + result.stderr)
    print("OK: tipos globais u32/u8 do Zuyu carregados com -I skyline em primeiro lugar")
