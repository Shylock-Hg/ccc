#! /usr/bin/env sh

clang-format -i --style=file \
$(find . -type f \( -name '*.c' -o -name '*.h' \))

