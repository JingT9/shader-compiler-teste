#!/usr/bin/env python3
"""Validate sources/API/CMake prerequisites before attempting the Android build."""
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
missing = []
for path in (
    "CMakeLists.txt", "src/shader_recompiler/CMakeLists.txt",
    "include/shader_compiler/frontend/ir/program.h",
    "include/shader_compiler/backend/spirv/emit_spirv.h",
    "include/shader_compiler/common/log.h",
    "include/shader_compiler/common/settings.h",
    "include/shader_compiler/common/common_types.h",
    "include/common/settings.h", "include/common/logging/log.h",
    "third_party/zuyu-original/src/common/common_types.h",
    "third_party/zuyu-original/src/common/assert.h",
    "third_party/zuyu-original/src/common/bit_field.h",
):
    if not (root / path).is_file():
        missing.append(path)
for path in missing:
    print("AUSENTE:", path, file=sys.stderr)
if missing:
    print("FALTAM dependencias originais; execute get_original_zuyu_common.sh", file=sys.stderr)
    sys.exit(1)
print("OK: estrutura para integracao encontrada; ainda exige build completo C++/Android")
