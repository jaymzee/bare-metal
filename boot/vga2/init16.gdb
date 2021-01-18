# use this script to start qemu, connect to it, and debug code as 16 bit
add-symbol-file boot.elf 0x7c00
set tdesc filename ../gdb/target.xml
target remote | ./qemu -S -gdb stdio
