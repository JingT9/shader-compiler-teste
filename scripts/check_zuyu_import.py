#!/usr/bin/env python3
"""Check Zuyu source immutability and SKYrus's expected compile-time layout."""
from pathlib import Path
import hashlib
import sys

root = Path(__file__).resolve().parents[1]
source = root / "src/shader_recompiler"
reference = root / "tests/zuyu-source.sha256"
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

# Compare the shader algorithms against immutable hashes of the unmodified Zuyu
# reference. A source snapshot is unnecessary: the main copy is the one in src/.
# Two public ABI headers and CMakeLists.txt are intentionally excluded.
count = 0
if reference.is_file():
    for line_no, line in enumerate(reference.read_text().splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        sha, separator, rel = line.partition("  ")
        if not separator or len(sha) != 64 or not all(c in "0123456789abcdef" for c in sha):
            errors.append(f"Registro SHA256 invalido na linha {line_no}")
            continue
        path = source / rel
        count += 1
        if not path.is_file():
            errors.append(f"Arquivo original ausente: {rel}")
        elif hashlib.sha256(path.read_bytes()).hexdigest() != sha:
            errors.append(f"Arquivo original modificado: {rel}")
    if count != 238:
        errors.append(f"Manifesto incompleto: {count} arquivos (esperados: 238)")
else:
    errors.append("Falta tests/zuyu-source.sha256 para comparar o codigo original")

for rel in ("common/log.h", "common/settings.h"):
    if not (root / "include/shader_compiler" / rel).is_file():
        errors.append(f"Interface SKYrus ainda ausente: {rel}")

if errors:
    for error in errors:
        print("FALTA:", error, file=sys.stderr)
    print(f"Verificacao parcial: {count} arquivos do Zuyu comparados", file=sys.stderr)
    sys.exit(1)
print(f"OK: {count} arquivos conferidos por SHA256 (exceto 2 cabecalhos de API e CMake); headers SKYrus presentes")
print("AVISO: estrutura verificada; compilacao e ABI ainda precisam de teste")
