#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/serial.h>
#include <sys/graphics.h>

// page table entry
#define PTE_ADDRMASK 0x000fffffFFFFF000
#define PTE_PRESENT  1
#define PTE_WRITABLE 2
#define PTE_USER     4
#define PTE_WRTHRU   8
#define PTE_NOCACHE  16
#define PTE_ACCESSED 32
#define PTE_DIRTY    64
#define PTE_HUGE     128
#define PTE_GLOBAL   256
#define PTE_NX       (1L << 63)


void DumpPTE(char *buf, uint64_t pte, int os_bits)
{
    char decode[64];
    int n = 0;

    // physical address (40 bits) display in hex
    decode[n++] = '0';
    decode[n++] = 'x';
    ltoa(pte & PTE_ADDRMASK, 16, 10, &decode[n]);
    n += 10;
    decode[n++] = ' '; // overwrites null

    // flags
    decode[n++] = pte & PTE_NX ? 'n' : 'x';       // no execute
    decode[n++] = pte & PTE_GLOBAL ? 'g' : '-';   // global
    decode[n++] = pte & PTE_HUGE ? 'h' : '-';     // huge page
    decode[n++] = pte & PTE_DIRTY ? 'd' : '-';    // page dirty
    decode[n++] = pte & PTE_ACCESSED ? 'a' : '-'; // page accessed
    decode[n++] = pte & PTE_NOCACHE ? '-' : 'c';  // disable cache
    decode[n++] = pte & PTE_WRTHRU ? 'W' : 'C';   // write thru caching
    decode[n++] = pte & PTE_USER ? 'u' : 'k';     // user accessible
    decode[n++] = pte & PTE_WRITABLE ? 'w' : '-';
    decode[n++] = pte & PTE_PRESENT ? 'p' : '-';
    decode[n++] = 0; // null terminator

    // os defined flags
    if (os_bits) {
        // os defined flags (62:52) display in binary
        decode[n++] = '0';
        decode[n++] = 'b';
        for (int i = 0; i < 11; i++) {
            decode[n++] = (pte >> (62-i)) & 1 ? '1' : '0';
        }
        decode[n++] = ' ';

        // os defined flags (11:9) display in binary
        decode[n++] = '0';
        decode[n++] = 'b';
        decode[n++] = pte & (1L << 11) ? '1' : '0';
        decode[n++] = pte & (1L << 10) ? '1' : '0';
        decode[n++] = pte & (1L << 9) ? '1' : '0';
        decode[n++] = ' ';
    }

    decode[n++] = 0; // null terminator

    strcpy(buf, decode);
}

void dump_pte(uint32_t pte) {
    char buf[128];

    DumpPTE(buf, pte, 0);
    println(buf);
}

void dump_page_tables()
{
    char nbuf[20];

    // level 4 page table (page map level 4 table)
    uint64_t *pml4t = (uint64_t *)0x4000;
    print("PML4T[0] = ");
    dump_pte(pml4t[0]); // PML4T[0] -> PDPT

    // level 3 page table (page directory pointer table)
    uint64_t *pdpt = (uint64_t *)(pml4t[0] & PTE_ADDRMASK);
    print("PDPT[0]  = ");
    dump_pte(pdpt[0]); // PDPT[0] -> PDT

    // level 2 page table (page directory table)
    uint64_t *pdt = (uint64_t *)(pdpt[0] & PTE_ADDRMASK);
    for (int i = 0; i < 4; i++) {
        print("PDT[i]   = ");
        dump_pte(pdt[i]); // PDT[i] -> PTi
    }
    for (int i = 0; i < 4; i++) {
        print("PDT[8+i] = ");
        dump_pte(pdt[8+i]); // PDT[i] -> PTi
    }

    // level 1 page table (identity mapped)
    uint64_t *pt = (uint64_t *)(pdt[0] & PTE_ADDRMASK);
    for (int i = 0; i < 4; i++) {
        print(ltoa(4096*i, 16, 8, nbuf));
        print(" -> ");
        dump_pte(pt[i]);
    }

    // level 1 page table (identity mapped)
    uint64_t *ptfb = (uint64_t *)(pdt[8] & PTE_ADDRMASK);
    for (int i = 0; i < 4; i++) {
        print(ltoa(0x200000 * 8 + 4096*i, 16, 8, nbuf));
        print(" -> ");
        dump_pte(ptfb[i]);
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
