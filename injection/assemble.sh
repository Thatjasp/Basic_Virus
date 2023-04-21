#! /bin/sh
nasm -f elf64 inject.S
ld -o inject inject.o -Ttext 0x401156 -m elf_x86_64
