# Validação da adaptação

Executada em 29/10/2026, com GCC 13.3.0 e C++20 em Linux x86_64.

## Verificações concluídas

- Compilação dos 182 arquivos `.cpp` do segundo compilador adaptado, incluindo
  os três backends, e dos 20 arquivos `.cpp` de Sirit: **202/202 aprovados**.
- Link dos executáveis de teste com a biblioteca estática completa.
- API do `Environment`, campos de perfil, vetor de transform feedback,
  compartilhamento das configurações entre dois arquivos C++, layout dos sete
  bindings e criação/reset/destruição dos objetos dos pools: **aprovados**.
- Tradução de instrução Maxwell real `EXIT`, otimização de IR e geração dos
  módulos abaixo: **aprovadas**. Os testes verificam cabeçalho, contagem de
  palavras e presença de entry point, função, label e retorno.

| SPIR-V | Estágio | Palavras |
| --- | --- | ---: |
| 1.0 | Compute | 210 |
| 1.0 | Vertex | 231 |
| 1.0 | Fragment | 207 |
| 1.4 | Compute | 210 |
| 1.4 | Vertex | 231 |
| 1.4 | Fragment | 207 |

- Todos os `.cpp` estão listados no CMake; os includes internos apontam para
  headers presentes no pacote.
- `git apply --check` do patch de integração no SKYrus anexado: **aprovado**.
- As cópias dos arquivos originais foram conferidas por SHA de blob do Git,
  antes das alterações.

Dependências usadas: fmt 9.1.0, headers Boost 1.84.0, range-v3 0.12.0,
Sirit `4ab79a8c023aa63caaa93848b09b9fe8b183b1a9` e SPIRV-Headers
`c214f6f2d1a7253bb0e9f195c2dc5b0659dc99ef`. São dependências externas;
este pacote não as inclui.

## Limites desta validação

A compilação local foi conduzida diretamente com GCC e headers pré-compilados.
O ambiente não dispõe de CMake, NDK, Rust, `spirv-val` ou dispositivo Vulkan.
Os workflows CMake/GitHub Actions e o APK completo do SKYrus não foram executados
aqui. Os seis shaders são casos mínimos; não constituem testes de jogos ou
garantia de compatibilidade gráfica em um aparelho.

A CI adicionada cobre CMake, GCC/Clang, `spirv-val` e a biblioteca Android arm64
quando o código for publicado. O código e o patch foram preparados localmente;
nenhum repositório remoto foi alterado, pois a conexão disponível permite apenas
leitura.
