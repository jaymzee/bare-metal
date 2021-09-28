# VESA graphics mode demo (64-bit long mode)
single stage bootloader puts CPU into long mode with paging and runs demo
- framebuffer is mapped to virtual address 0x04000000
- display is put into VESA mode 144h (1024x768x32)
- some colored lines are drawn directly to the framebuffer

memory map

    0x04000000 0x04ffffff framebuffer (maps to phys addr 0xfd000000)
    0x00f00000 0x00ffffff stack for protected mode 1M
    0x00040000 0x00eFFFFF program memory (14.75MB)
    0x00028000 0x0002ffff PTs for framebuffer (16MB)
    0x00010000 0x00027fff PTs for system memory (16 - 64MB) reserved
    0x00008000 0x0000ffff PTs for system memory (0 - 16MB)
    0x00007e00 0x00007fff stage 1 bootblock
    0x00007c00 0x00007dff stage 0 bootblock
    0x00007000 0x00007bff stack for real mode
    0x00006000 0x00006fff PDT
    0x00005000 0x00005fff PDPT
    0x00004000 0x00004fff PML4T
    0x00003000 0x00003fff GDT
    0x00002400 0x000025ff VESA mode info
    0x00002200 0x000023ff VESA info
    0x00002100 0x00002101 VESA mode
    0x00002000 0x00002009 IDTR
    0x00001000 0x00001fff IDT
    0x00000000 0x000003ff IVT, not mapped in protected mode
