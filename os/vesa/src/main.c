#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/serial.h>
#include <sys/graphics.h>

void dump_page_tables()
{
    char nbuf[20];

    uint64_t *pml4t = (uint64_t *)0x4000;
    puts(ltoa(pml4t[0], 16, 16, nbuf)); // PML4T[0] -> PDPT

    uint64_t *pdpt = (uint64_t *)(pml4t[0] & 0xFFFFF000);
    puts(ltoa(pdpt[0], 16, 16, nbuf)); // PDPT[0] -> PDT

    uint64_t *pdt = (uint64_t *)(pdpt[0] & 0xFFFFF000);
    for (int i = 0; i < 8; i++) {
        puts(ltoa(pdt[i+8], 16, 16, nbuf)); // PDT[i] -> PTi
    }

    uint64_t *pt = (uint64_t *)(pdt[8] & 0xFFFFF000);
    for (int i = 0; i < 8; i++) {
        puts(ltoa(pt[i], 16, 16, nbuf));
    }
}

int main(int argc, char *argv[], char *envp[])
{
    dump_page_tables();

    println("Hi resolution graphics in long mode (64-bit) demo");

    // draw some lines
    DrawLine32(0, 0, 1024, 768, 0x00FF00);
    DrawLine32(0, 768, 1024, 0, 0xFF0000);
    DrawLine32(0, 384, 1024, 384, 0x0000FF);
    DrawLine32(512, 0, 512, 768, 0xFFFF00);

    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
