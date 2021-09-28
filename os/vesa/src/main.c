#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cpu.h>
#include <sys/io.h>
#include <sys/serial.h>
#include <sys/lfb/draw.h>
#include <sys/vesa.h>
#include <stdint.h>

void print_page_tables()
{
    uint64_t *pml4t = (uint64_t *)0x4000;
    uint64_t *pdpt0 = (uint64_t *)(pml4t[0] & PTE_ADDRMASK);
    uint64_t *pdt0 = (uint64_t *)(pdpt0[0] & PTE_ADDRMASK);

    // page map level 4 entries
    PrintPTE(stdout, "PML4T[0]      = ", pml4t[0]);
    // page directory pointer table entries
    PrintPTE(stdout, "PDPT[0]       = ", pdpt0[0]);
    // page directory table entries (page table locations)
    PrintPDT(stdout, pdt0, 0, 4);
    // page directory table entries (page table locations for framebuffer)
    PrintPDT(stdout, pdt0, 32, 36);
    // first 4 page table entries
    PrintPT(stdout, pdt0, 0, 0, 4);
    // first 4 page table entries (for framebuffer)
    PrintPT(stdout, pdt0, 32, 0, 4);
}

void init_graphics() {
    char nbuf[20];

    int *mode = (int *)0x2100;
    struct VbeInfo *vbi = (struct VbeInfo *)0x2200;
    struct VbeModeInfo *vmi = (struct VbeModeInfo *)0x2400;

    print("video ram: ");
    println(ltoa((long)vbi->TotalMemory << 16, 10, 1, nbuf));
    print("video mode: 0x");
    print(itoa(*mode & 0xFFF, 16, 3, nbuf));
    print("  ");
    print(itoa(vmi->Xres, 10, 1, nbuf));
    print(" x ");
    print(itoa(vmi->Yres, 10, 1, nbuf));
    print(" x ");
    print(itoa(vmi->bpp, 10, 1, nbuf));
    print("  0x");
    println(ltoa(vmi->physbase, 16, 8, nbuf));
    LFB_Init((uint32_t *)0x04000000, vmi->Xres);
}

int main(int argc, char *argv[], char *envp[])
{
    init_graphics();
    print_page_tables();

    println("Hi resolution graphics in long mode (64-bit) demo");

    // draw some lines
    LFB_DrawLine(0, 0, 1024, 768, 0x00FF00);
    LFB_DrawLine(0, 768, 1024, 0, 0xFF0000);
    LFB_DrawLine(0, 384, 1024, 384, 0x0000FF);
    LFB_DrawLine(512, 0, 512, 768, 0xFFFF00);

    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
