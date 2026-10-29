#!/usr/bin/env python3
"""Reproduce the SKYrus/Zuyu common_funcs.h include-order collision."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[1]
wrapper = root / 'src/shader_recompiler/common/common_funcs.h'
original = root / 'third_party/zuyu-original/src/common/common_funcs.h'
program = root / 'src/shader_recompiler/program_header.h'

for path in (wrapper, original, program):
    if not path.is_file():
        sys.exit(f'MISSING: {path} (run: bash scripts/get_original_zuyu_common.sh)')

# Check we are forwarding to the original file and not shadowing host helpers.
expected = (wrapper.parent / '../../../third_party/zuyu-original/src/common/common_funcs.h').resolve()
if expected != original.resolve():
    sys.exit(f'Invalid relative path in common_funcs.h: {expected}')

compiler = next((c for name in ('clang++', 'c++', 'g++') if (c := shutil.which(name))), None)
if not compiler:
    sys.exit('No C++ compiler available for the regression test')

with tempfile.TemporaryDirectory(prefix='skyrus-zuyu-include-') as td:
    temp = Path(td)
    host = temp / 'host/common'
    host.mkdir(parents=True)
    # A host common_funcs.h exists earlier in -I, but has no Zuyu padding macros.
    (host / 'common_funcs.h').write_text('#pragma once\n')
    test = temp / 'test.cpp'
    test.write_text('''#include "shader_recompiler/program_header.h"
#ifndef INSERT_PADDING_BYTES_NOINIT
#error Zuyu common_funcs.h was shadowed by SKYrus common_funcs.h
#endif
#ifndef INSERT_PADDING_WORDS_NOINIT
#error Zuyu common_funcs.h was shadowed by SKYrus common_funcs.h
#endif
static_assert(sizeof(Shader::ProgramHeader) == 0x50);
int main() { return 0; }
''')
    cmd = [compiler, '-std=c++20', '-fsyntax-only',
           '-I' + str(temp / 'host'),
           '-I' + str(root / 'src'),
           '-I' + str(root / 'src/shader_recompiler'),
           '-I' + str(root / 'third_party/zuyu-original/src'),
           str(test)]
    result = subprocess.run(cmd, text=True, capture_output=True)
    if result.returncode:
        sys.stderr.write(result.stdout + result.stderr)
        sys.exit(result.returncode)

print('OK: ProgramHeader compiles with SKYrus common/ first in the include search path')
