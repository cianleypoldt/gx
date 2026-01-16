#!/usr/bin/env bash
set -e

# Regenerate compile_commands.json for clangd if --bear flag is passed
if [ "$1" = "--bear" ]; then
    bear -- gcc -c main.c renderer/context.c renderer/shader.c renderer/layout.c renderer/mesh.c renderer/camera.c core/array.c core/mat4.c core/platform.c core/file.c external/glad.c -I. -Iexternal
    echo "Generated compile_commands.json"
fi

gcc main.c renderer/context.c renderer/shader.c renderer/layout.c renderer/mesh.c renderer/camera.c core/array.c core/mat4.c core/platform.c core/file.c external/glad.c -lglfw -lm -I. -Iexternal -o gx && ./gx
