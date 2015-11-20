#!/bin/csh -f

(setenv PKG_CONFIG_PATH ${PKG_CONFIG_PATH}:${HOME}/workspace/liblog/build-$1-${ARCH}/install/lib/pkgconfig; ../configure -C CXX="ccache-clang++" CC="ccache-clang" --prefix=${HOME}/workspace/librp/build-$1-${ARCH}/install --enable-maintainer-mode --enable-silent-rules --enable-logging --enable-debug)
