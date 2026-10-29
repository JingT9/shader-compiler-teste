PATCH 0010 - Fix Zuyu ProgramHeader INSERT_PADDING_BYTES_NOINIT

The 2026-10-29 Android build stops at src/shader_recompiler/program_header.h:72
because INSERT_PADDING_BYTES_NOINIT is not visible. This patch adds an include
shim at src/shader_recompiler/common/common_funcs.h that forwards by relative
path to the *unaltered* Zuyu original common_funcs.h at:
  third_party/zuyu-original/src/common/common_funcs.h

It does not modify any shader algorithms, ProgramHeader, or the original Zuyu
common implementation. The regression test compiles ProgramHeader with a
competing SKYrus common/common_funcs.h earlier on the -I search path.

Requirements: run bash scripts/get_original_zuyu_common.sh beforehand.

Commands:
  git apply --check /sdcard/Download/0010-zuyu-fix-common-funcs-padding.patch
  git apply --index /sdcard/Download/0010-zuyu-fix-common-funcs-padding.patch
  python3 tests/test_common_funcs_priority.py
  python3 scripts/check_zuyu_import.py

This is a targeted fix, NOT proof that the full Android build passes.
