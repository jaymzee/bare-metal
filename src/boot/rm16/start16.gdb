add-symbol-file stage1.elf 0x7e00
add-symbol-file stage0.elf 0x7c00
set tdesc filename ../../gdb/target.xml
target remote | ./qemu -S -gdb stdio
