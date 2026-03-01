#!/usr/bin/env bash
set -e

# Collect all C files recursively
C_FILES=$(find . -name "*.c" | sort)

# Regenerate compile_commands.json for clangd if --bear flag is passed
if [ "$1" = "--bear" ]; then
    bear -- gcc -c $C_FILES -I. -Iexternal
    echo "Generated compile_commands.json"
fi

gcc -Wall -Wextra $C_FILES -lglfw -lm -I. -Iexternal -o gx && ./gx
