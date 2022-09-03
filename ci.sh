#!/bin/sh

./build.sh || exit 1
# HACK: VS2022 と MSYS2 で、フォルダの位置を合わせる。
pushd vs/test
../../src/test || exit 1
popd
