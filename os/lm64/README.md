## Long Mode 64 demo
single stage bootloader puts CPU into long mode, then executes the 64-bit demo
- serial and vga console
- demonstrate system timer and keyboard interrupt handling
- demonstrate CPU Exception handling
- beep from pc speaker

memory map

    0x02000000 0x02000fff local APIC registers
    0x00f00000 0x00ffffff stack for protected mode 1M
    0x00040000 0x00eFFFFF program memory (14.75MB)
    0x00030000 0x00030fff local APIC PT
    0x0000f000 0x0000ffff PT 14 - 16MB of memory
    0x0000e000 0x0000efff PT 12 - 14MB of memory
    0x0000d000 0x0000dfff PT 10 - 12MB of memory
    0x0000c000 0x0000cfff PT 8 - 10MB of memory
    0x0000b000 0x0000bfff PT 6 - 8MB of memory
    0x0000a000 0x0000afff PT 4 - 6MB of memory
    0x00009000 0x00009fff PT 2 - 4MB of memory
    0x00008000 0x00008fff PT 0 - 2MB of memory
    0x00007e00 0x00007fff stage 1 bootblock
    0x00007c00 0x00007dff stage 0 bootblock
    0x00007000 0x00007bff stack for real mode
    0x00006000 0x00006fff PDT0
    0x00005000 0x00005fff PDPT
    0x00004000 0x00004fff PML4T
    0x00003000 0x00003fff GDT
    0x00002000 0x00002009 IDTR
    0x00001000 0x00001fff IDT
    0x00000000 0x000003ff IVT, not mapped in protected mode
