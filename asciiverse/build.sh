#!/bin/sh
# Builds gnuski. Linux: run inside alpine:3.22 for a fully static binary
# (ncurses linked in); macOS: run on the runner with CC_ARCH set to x86_64
# or arm64, linking the system's ncurses.
#   docker run --rm -v "$PWD":/src -w /src alpine:3.22 sh asciiverse/build.sh
# Leaves ./gnuski. Upstream's Makefile is one gcc line; its variables are
# overridden here rather than patched.
set -eu
if [ -f /etc/alpine-release ]; then
  apk add --no-cache build-base ncurses-dev ncurses-static >/dev/null
  libs="-static -lncursesw"
else
  libs="-lncurses"
fi
cc="cc${CC_ARCH:+ -arch $CC_ARCH}"
rm -f gnuski
make CC="$cc" FLAGS="-O2 -Wall -o gnuski" LIBS="$libs"
strip gnuski
