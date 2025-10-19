#!/bin/bash

nasm -felf64 out.asm
ld out.o -o out
