# Zuyu original no shader-compiler-teste

A finalidade destes patches e somente preparar a **integracao** do codigo
original `src/shader_recompiler` com o alvo `shader_recompiler` consumido pelo
SKYrus. Nenhum `.cpp` ou `.h` existente no ZIP original e alterado.

Este primeiro passo preserva as duas grafias de include:

- `shader_recompiler/...`: usada internamente pelo Zuyu;
- `shader_compiler/...`: usada pelo SKYrus.

Execute `python3 scripts/prepare_exports.py` para criar as entradas de CMake e
os aliases sem sobrescrever arquivos ja existentes. O segundo patch adiciona
as interfaces de log e configuracao e um procedimento de importacao da dependencia
`common` do mesmo Zuyu original.

O alvo original tambem requer `fmt::fmt`, `sirit` e `common`. **CMake configurado
nao e o mesmo que biblioteca compilada com sucesso**, e so se deve atualizar o
submodulo do SKYrus depois de validar a API e o build Android.

## Dependencias originais (segundo patch)

`python3 scripts/check_zuyu_import.py` compara o codigo original com a copia
`zuyu-original` e verifica os aliases. Os adapters de log/configuracao ficam em
`common/`, fora dos fontes originais; nenhum algoritmo de shader e modificado.

A configuracao `Settings::values` do Zuyu e `Shader::Settings::values` do SKYrus
agora aponta ao mesmo estado. Para recuperar os demais arquivos originais
`common/...` execute `bash scripts/get_original_zuyu_common.sh` **e revise os
arquivos recuperados antes de adiciona-los ao Git**. Esses arquivos carregam
suas proprias licencas e podem exigir outras dependencias do Zuyu.

Teste de alias de configuracao (C++20):

```sh
c++ -std=c++20 -Iinclude -Isrc tests/settings_alias.cpp -o /tmp/zuyu-settings && /tmp/zuyu-settings
```

Isto **nao e um teste de compilacao do APK**. O compilador ainda depende da
biblioteca `common`, de `fmt::fmt` e `sirit`, e as interfaces entre versoes
Zuyu / SKYrus podem divergir. Nao atualize o submodulo ate validar essas APIs.
