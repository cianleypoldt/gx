#!/usr/bin/env bash
set -e
gcc main.c platform.c external/glad.c -lglfw -lm -o gl_trial && ./gl_trial
