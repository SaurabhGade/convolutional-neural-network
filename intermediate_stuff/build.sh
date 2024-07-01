#!/bin/sh
set -xe
gcc -Wall -Wextra -o nn nn.c -lm
./nn
