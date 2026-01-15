#!/usr/bin/env bash
set -e

# Regenerate compile_commands.json for clangd if --bear flag is passed
if [ "$1" = "--bear" ]; then
    bear -- gcc -c main.c renderer/context.c renderer/shader.c core/array.c core/platform.c core/file.c external/glad.c -I. -Iexternal
    echo "Generated compile_commands.json"
fi

gcc main.c renderer/context.c renderer/shader.c core/array.c core/platform.c core/file.c external/glad.c -lglfw -lm -I. -Iexternal -o gx && ./gx
