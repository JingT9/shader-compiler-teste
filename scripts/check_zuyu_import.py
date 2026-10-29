#!/usr/bin/env python3
"""Check Zuyu source immutability and SKYrus's expected compile-time layout."""
from pathlib import Path
import hashlib
import sys

root = Path(__file__).resolve().parents[1]
source = root / "src/shader_recompiler"
reference = root / "zuyu-original"
required = (
    "backend/bindings.h", "backend/spirv/emit_spirv.h",
    "frontend/ir/basic_block.h", "frontend/ir/program.h",
    "frontend/ir/value.h", "frontend/maxwell/control_flow.h",
    "frontend/maxwell/translate_program.h", "host_translate_info.h",
    "object_pool.h", "profile.h", "runtime_info.h", "shader_info.h",
)
errors = []
for rel in required:
    if not (source / rel).is_file():
        errors.append(f"Fonte original ausente: {rel}")
    if not (root / "include/shader_compiler" / rel).is_file():
        errors.append(f"Alias SKYrus ausente: {rel}")

count = 0
if reference.is_dir():
    for old in reference.rglob("*"):
        if old.is_file() and old.name != "CMakeLists.txt":
            rel = old.relative_to(reference)
            new = source / rel
            count += 1
            if not new.is_file() or hashlib.sha256(old.read_bytes()).digest() != hashlib.sha256(new.read_bytes()).digest():
                errors.append(f"Arquivo original modificado/ausente: {rel}")
else:
    errors.append("Falta zuyu-original/ para comparar o codigo original")

for rel in ("common/log.h", "common/settings.h"):
    if not (root / "include/shader_compiler" / rel).is_file():
        errors.append(f"Interface SKYrus ainda ausente: {rel}")

if errors:
    for error in errors:
        print("FALTA:", error, file=sys.stderr)
    print(f"Verificacao parcial: {count} arquivos do Zuyu comparados", file=sys.stderr)
    sys.exit(1)
print(f"OK: {count} arquivos originais intactos; headers SKYrus presentes")
print("AVISO: estrutura verificada; compilacao e ABI ainda precisam de teste")
