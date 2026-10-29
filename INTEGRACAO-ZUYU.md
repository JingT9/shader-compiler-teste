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

## Etapa 3 — adaptadores para compilacao do SKYrus

- Requer **as dependencias `common` originais** importadas pelo script do passo 2.
- Exporta `common` como biblioteca INTERFACE somente quando a build pai nao a
  fornece. Os algoritmos continuam sendo os fontes originais do Zuyu.
- Desvia as mensagens de logging para `Shader::Log` e implementa as duas
  chamadas de falha (`assert_fail_impl` e `unreachable_impl`) com excecoes.
  Isto afeta somente a integracao/erros de execucao, nao os calculos de shader.
- `Environment::Dump` encaminha a assinatura de dois hashes do Zuyu para a
  callback de um hash do SKYna, mantendo a API do host. A grafia antiga
  `IsPropietaryDriver` tambem esta disponivel como alias.
- `HostTranslateInfo::needs_byte_swap_workaround` foi acrescentado apenas para
  compatibilidade com a estrutura publica usada pelo SKYrus; a implementacao
  de traducao original do Zuyu nao foi modificada.
- Execute `bash scripts/get_original_zuyu_common.sh`, depois
  `python3 scripts/check_build_integration.py`.
- `fmt::fmt` e `sirit` precisam vir da build hospedeira (SKYrus).

**Limite:** essas adaptacoes NAO comprovam a compatibilidade total da ABI,
que so pode ser verificada compilando a arvore real do SKYrus e executando
jogos. Um build que falhar deve mostrar o erro C++ real; nao aplique fixes
visuais ou alteracoes de algoritmo como resposta automatica.
