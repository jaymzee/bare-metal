# bootloader that switches to protected mode and runs 32-bit C code

memory map

    0x00f00000 0x00ffffff stack for protected mode 1M
    0x00010000 0x00efffff program memory (15M - 64K)
    0x00007e00 0x00007fff stage 1 boot block
    0x00007c00 0x00007dff stage 0 boot block
    0x00007000 0x00007bff stack for real mode 3K
    0x00002000 0x00002fff GDT
    0x00001000 0x000017ff IDT (protected mode)
    0x00000000 0x000003ff IVT (real mode)
