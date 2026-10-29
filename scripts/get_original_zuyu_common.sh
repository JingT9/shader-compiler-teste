#!/usr/bin/env bash
# Import the unchanged 'common' dependency from the same archived Zuyu source.
# Only headers/sources are copied; no shader implementation is touched.
set -euo pipefail
root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target="$root/third_party/zuyu-original/src/common"
source_repo="https://github.com/uristrimber/zuyu.git"
source_rev="027f64368e5d180c3fd1c35315a044a36162be69"
if [[ -e "$target/common_types.h" ]]; then
  echo "Os fontes originais de common ja existem: $target"
  exit 0
fi
mkdir -p "$root/third_party/zuyu-original/src"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
git clone --quiet --filter=blob:none --no-checkout "$source_repo" "$tmp/zuyu"
git -C "$tmp/zuyu" sparse-checkout init --cone
git -C "$tmp/zuyu" sparse-checkout set src/common
git -C "$tmp/zuyu" checkout --quiet "$source_rev"
cp -R "$tmp/zuyu/src/common" "$target"
echo "Importado: common original do Zuyu @ $source_rev"
echo "AVISO: fontes de common nao sao um build independente; dependencias/link ainda precisam validacao."
