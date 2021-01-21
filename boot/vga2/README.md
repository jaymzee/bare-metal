# bootloader that set CPU to long mode with paging and runs 64-bit C code
program sets display to vesa mode 144h and writes directly to framebuffer 0xfb000000

memory map
    0x01000000 0x01ffffff framebuffer (maps to phys addr 0xfd000000)
    0x00f00000 0x00ffffff stack for protected mode 1M
    0x00040000 0x00eFFFFF program memory (15M - 256K)
    0x00017000 0x00017fff PT for framebuffer (14 - 16MB)
    0x00016000 0x00016fff PT for framebuffer (12 - 14MB)
    0x00015000 0x00015fff PT for framebuffer (10 - 12MB)
    0x00014000 0x00014fff PT for framebuffer (8 - 10MB)
    0x00013000 0x00013fff PT for framebuffer (6 - 8MB)
    0x00012000 0x00012fff PT for framebuffer (4 - 6MB)
    0x00011000 0x00011fff PT for framebuffer (2 - 4MB)
    0x00010000 0x00010fff PT for framebuffer (0 - 2MB)
    0x0000f000 0x0000ffff PT for system memory (14 - 16MB)
    0x0000e000 0x0000efff PT for system memory (12 - 14MB)
    0x0000d000 0x0000dfff PT for system memory (10 - 12MB)
    0x0000c000 0x0000cfff PT for system memory (8 - 10MB)
    0x0000b000 0x0000bfff PT for system memory (6 - 8MB)
    0x0000a000 0x0000afff PT for system memory (4 - 6MB)
    0x00009000 0x00009fff PT for system memory (2 - 4MB)
    0x00008000 0x00008fff PT for system memory (0 - 2MB)
    0x00007e00 0x00007fff stage 1 bootblock
    0x00007c00 0x00007dff stage 0 bootblock
    0x00007000 0x00007bff stack for real mode
    0x00006000 0x00006fff PDT
    0x00005000 0x00005fff PDPT
    0x00004000 0x00004fff PML4T
    0x00003000 0x00003fff GDT
    0x00002000 0x00002009 IDTR
    0x00001000 0x00001fff IDT
    0x00000000 0x000003ff IVT, not mapped in protected mode
