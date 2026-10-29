Patch 0009 - Fix do primeiro erro real do APK em 29/10/2026

Falha do log: program_header.h:15:29: unknown type name 'u32' (mesma causa para u8)

O adaptador src/shader_recompiler/common/common_types.h antes usava:
  #include <common/common_types.h>

A linha real do clang da build Android coloca o diretorio de includes do
SKYrus antes do diretorio com os tipos originais do Zuyu. O include generico
pode assim resolver o arquivo homonimo do projeto pai, sem os typedefs
u32/u8 globais. O patch usa o header original via caminho relativo a este
adaptador; nao muda nem um byte de shader, SPIR-V, GLSL ou GLASM.

Aplique apenas no repositorio shader-compiler-teste (branch zuyu-importacao).
Antes, a dependencia third_party/zuyu-original precisa ser preparada pelo
script scripts/get_original_zuyu_common.sh, como na build anterior.

Validacao local:
  python3 tests/test_common_types_priority.py
  python3 scripts/check_zuyu_import.py

Depois de subir o patch, execute novamente o GitHub Actions do SKYrus de
testes. A validacao acima e especifica para a colisao de includes: pode haver
outros erros C++ mais adiante que so o build real revelara.
