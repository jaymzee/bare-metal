# use this script to start qemu, connect to it, and debug code as 32/64 bit
add-symbol-file main.elf 0x10000
add-symbol-file boot.elf 0x7c00
target remote | ./qemu -S -gdb stdio
