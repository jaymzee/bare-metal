#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/serial.h>

uint8_t *vptr = (uint8_t *)0xfd000000;

int main(int argc, char *argv[], char *envp[])
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

    println("Hi resolution graphics in long mode (64-bit) demo");
    uint64_t *p64 = (uint64_t *)0x1000000;
    *p64 = 0xFFFFFFFF;
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
