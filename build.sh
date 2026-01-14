#!/usr/bin/env bash
set -e

# Regenerate compile_commands.json for clangd if --bear flag is passed
if [ "$1" = "--bear" ]; then
    bear -- gcc -c main.c core/gx.c core/utils.c external/glad.c -I. -Iexternal
    echo "Generated compile_commands.json"
fi

gcc main.c gx.c core/array.c core/platform.c external/glad.c -lglfw -lm -I. -Iexternal -o gx && ./gx
