# Protected mode 32-bit demo
single stage bootloader puts CPU into protected mode and executes 32-bit demo
- serial and vga console
- system timer and keyboard interrupt handling
- demonstrate CPU exception handling (register dump)

memory map

    0x00f00000 0x00ffffff stack for protected mode 1M
    0x00040000 0x00efffff program memory (14.75MB)
    0x00007e00 0x00007fff stage 1 boot block
    0x00007c00 0x00007dff stage 0 boot block
    0x00007000 0x00007bff stack for real mode 3K
    0x00002000 0x00002fff GDT
    0x00001000 0x000017ff IDT (protected mode)
    0x00000000 0x000003ff IVT (real mode)
