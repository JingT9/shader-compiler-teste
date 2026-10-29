# SKYrus Shader Compiler

Adaptação de [`JingT9/shader-compiler-teste`](https://github.com/JingT9/shader-compiler-teste)
para substituir o submódulo de shaders do SKYrus. A tradução Maxwell, a
otimização de IR e a emissão SPIR-V continuam em C++; a fronteira com Rust
permanece no SKYrus.

## Integração no SKYrus

Depois de publicar esta adaptação em `main` no repositório acima, use-o em
`app/libraries/shader-compiler`. O CMake do SKYrus continua usando:

```cmake
add_subdirectory("libraries/shader-compiler")
target_link_libraries(skyline PRIVATE shader_recompiler)
```

O target exporta `include/` e suas dependências públicas. Headers como
`<shader_compiler/environment.h>` são arquivos reais, sem depender de symlinks.
Também está disponível o alias `SKYrus::ShaderCompiler`.

O arquivo [integration/skyrus.patch](integration/skyrus.patch) foi preparado e
conferido contra o SKYrus anexado. Ele troca a URL do submódulo e atualiza a
descoberta dos headers nos nove runners nativos do projeto. Para aplicá-lo, na
raiz de um checkout Git do SKYrus:

```bash
git apply --check /caminho/para/shader-compiler-teste/integration/skyrus.patch
git apply /caminho/para/shader-compiler-teste/integration/skyrus.patch
git submodule sync -- app/libraries/shader-compiler
git submodule update --init --recursive --remote -- app/libraries/shader-compiler
git -C app/libraries/shader-compiler checkout COMMIT_DA_ADAPTACAO_PUBLICADA
git add .gitmodules app/CMakeLists.txt app/src/main/rust/gpu/tests app/libraries/shader-compiler
```

Substitua `COMMIT_DA_ADAPTACAO_PUBLICADA` pelo commit real já publicado.
`--remote` é usado nesta migração para obter a `main` do novo repositório; o
`checkout` e o `git add` registram o commit exato a ser usado nos próximos builds.
A troca da URL em `.gitmodules` não muda sozinha o commit do submódulo, e o commit
antigo pode não existir no novo repositório. O SKYrus anexado usa NDK
`26.1.10909125`.

## Compatibilidade preservada

- Target `shader_recompiler`, namespace `Shader` e includes `shader_compiler/...`.
- `Environment::Dump(u64)`, o membro protegido `is_propietary_driver` e as duas
  grafias do accessor de driver. O overload com dois hashes encaminha ao primeiro.
- Campos de `HostTranslateInfo` e `Profile` usados pelo `ShaderManager`, com os
  workarounds Vulkan do compilador original para drivers móveis.
- `RuntimeInfo::xfb_varyings` como vetor e os sete `u32` de `Backend::Bindings`,
  na ordem esperada pelo cache C++/Rust.
- Lista completa de `TexturePixelFormat` do segundo repositório, acrescida de
  `OTHER`; backends GLSL e GLASM preservados.

As configurações agora usam uma única variável `inline` entre emulador e
compilador. Os pools destroem os objetos ativos antes de liberar a memória, e
`Common::BitCast` usa `std::bit_cast`. A dependência do target `common` do outro
emulador foi removida. Range-v3 fornece os algoritmos usados com o NDK do SKYrus.

O host continua fornecendo `Shader::Log::Debug`, `Warn` e `Error`, como já faz o
`ShaderManager` do SKYrus.

## Build e testes

Requer C++20, CMake 3.16+, fmt 8+, Boost, range-v3 e o Sirit usado pelo SKYrus.
Ao incorporar a biblioteca com `add_subdirectory`, os targets já criados pelo
emulador são reutilizados. Para um build nativo separado, instale os pacotes de
desenvolvimento de fmt, Boost e range-v3 e obtenha Sirit com seus submódulos:

```bash
git clone --recursive https://github.com/strato-emu/sirit.git /caminho/para/sirit
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DSKYRUS_SHADER_SIRIT_SOURCE_DIR=/caminho/para/sirit \
  -DSKYRUS_SHADER_BUILD_TESTS=ON
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

`SKYRUS_SHADER_USE_PRECOMPILED_HEADERS=ON` habilita PCH. Não há downloads
implícitos no CMake. Os testes verificam a API consumida pelo SKYrus, o
compartilhamento das configurações entre unidades C++, o layout dos bindings,
o ciclo de vida dos pools e o caminho Maxwell → IR → SPIR-V para compute, vertex
e fragment em SPIR-V 1.0 e 1.4.

A CI incluída configura builds nativos com GCC/Clang, validação com `spirv-val`
e compilação Android arm64 com o NDK do SKYrus. Esses jobs só serão executados
depois de o workflow ser publicado no GitHub. Veja [VALIDACAO.md](VALIDACAO.md)
para os testes efetivamente executados nesta entrega.

## Origem

Base: `shader-compiler-teste` em `6e5d1f65d64af9699222a56a4d5cac0c28360415`.
As adaptações de Vulkan e da API do SKYrus foram portadas de
[`Shader-Compiler-SKYna`](https://github.com/JingT9/Shader-Compiler-SKYna), em
`4159bfd8342f9b18f9788d981474f427e3471964`. Os avisos de autoria e as licenças
dos arquivos existentes foram preservados. O código do recompilador é
GPL-2.0-or-later; os shims provenientes do Skyline mantêm seus avisos MPL-2.0.
