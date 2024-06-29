#!/bin/sh
set -xe   ##Enable tracing , so it will show what is running
gcc -Wall -Wextra main.c  -o main #Wall -- show warning   -Wextra -- show all extra warnings.

./main
