# 16-bit real mode demo
Two stage bootloader loads and runs demo
- serial and vga console
- query CMOS real time clock
- query VESA VGA modes
- set VESA VGA mode to 0x118 (1024x768x24)


# memory map

0x000f0000 0x000fffff Motherboard BIOS
0x000c8000 0x000effff BIOS expansions
0x000c0000 0x000c7fff Video BIOS
0x000a0000 0x000bffff Video display memory
0x00080000 0x0009ffff Extended BIOS data area
0x00007e00 0x0007ffff Conventional memory ~480KB
0x00007c00 0x00007dff OS boot sector
0x00000500 0x00007bff Conventional memory ~30KB
0x00000400 0x000004ff BIOS data area
0x00000000 0x000003ff IVT
