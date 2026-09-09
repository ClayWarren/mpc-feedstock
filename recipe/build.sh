#!/bin/bash

# Get an updated config.sub and config.guess
cp $BUILD_PREFIX/share/gnuconfig/config.* build-aux/ || true

if [[ "$target_platform" == win-* ]]; then
  export PREFIX=${PREFIX}/Library
fi

extra_configure_args=()
if [[ "$target_platform" == "win-arm64" ]]; then
  set -e
  # MSYS2 runs under x64 emulation; the compiler and all tests are native ARM64.
  extra_configure_args=(--build=aarch64-pc-mingw32 --host=aarch64-pc-mingw32)
  export CFLAGS="${CFLAGS} -std=gnu17"
  export PATH="$PWD/src/.libs:$PATH"
fi

./configure --prefix=$PREFIX "${extra_configure_args[@]}" \
            --with-gmp=$PREFIX \
            --with-mpfr=$PREFIX \
            --disable-static

make -j${CPU_COUNT}
if [[ "$CONDA_BUILD_CROSS_COMPILATION" != 1 ]]; then
  make check -j${CPU_COUNT}
fi
make install

if [[ "$target_platform" == "win-64" ]]; then
  cp ${PREFIX}/lib/libmpc.dll.a ${PREFIX}/lib/mpc.lib
fi

if [[ "$target_platform" == "win-arm64" ]]; then
  # Native MSVC-style libtool installs its DLL beside the import library.
  mkdir -p "$PREFIX/bin"
  mv "$PREFIX/lib/mpc-3.dll" "$PREFIX/bin/mpc-3.dll"
  mv "$PREFIX/lib/mpc.dll.lib" "$PREFIX/lib/mpc.lib"
  test -f "$PREFIX/bin/mpc-3.dll"
  test -f "$PREFIX/lib/mpc.lib"
fi
