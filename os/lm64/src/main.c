#include <stdio.h>
#include <string.h>
#include <sys/interrupt.h>
#include <sys/test.h>
#include <sys/timer.h>

#include <sys/cpu.h>

#include "isr.h"

#define CS 0x08
#define DPL 0

void InstallISRs(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDTR *idtr = (void *)(IDTR_ADDRESS);

    IDT_IntGate(idt + 0x21, KeyboardHandler, CS, DPL);
    idtr->limit = 0x22 * sizeof(struct IDT_entry) - 1;

    LoadIDT(idtr); // also enables cpu interrupts

    PIC_UnmaskIRQ(1); // enable keyboard IRQ
}

void Menu(void)
{
    char sbuf[32];

    println("long mode demo");
    while (1) {
        print("press a key ");
        int ch = getchar();
        println("");
        print("system timer: ");
        println(ltoa(system_time, 10, 0, sbuf));
        switch (ch) {
        case 'b':
            Beep(1000);
            usleep(100*1000);
            NoSound();
            break;
        case 'd':
            CauseDivbyzero();
            break;
        case 'p':
            CausePageFault();
            break;
        }
    }
}

void ShowCPUID(void)
{
    char buf[80];
    union cpuid_result result;

    _cpuid(&result.reg, 0);
    println("CPUID EAX=0 highest function and mfr string");
    print("EAX: ");
    println(itoa(result.reg.eax, 16, 8, buf));
    strncpy(buf, result.str, 12);
    buf[12] = '\0';
    print("string: ");
    println(buf);
    _cpuid(&result.reg, 1);
    println("CPUID EAX=1 processor info and features");
    print("EAX: ");
    println(itoa(result.reg.eax, 16, 8, buf));
    print("EBX: ");
    println(itoa(result.reg.ebx, 16, 8, buf));
    print("ECX: ");
    println(itoa(result.reg.ecx, 16, 8, buf));
    print("EDX: ");
    println(itoa(result.reg.edx, 16, 8, buf));
}

void dump_page_tables()
{
    uint64_t *pml4t = (uint64_t *)0x4000;
    uint64_t *pdpt0 = (uint64_t *)(pml4t[0] & PTE_ADDRMASK);
    uint64_t *pdt0 = (uint64_t *)(pdpt0[0] & PTE_ADDRMASK);

    // page map level 4 entries
    PrintPTE(console, "PML4T[0]      = ", pml4t[0]);
    // page directory pointer table entries
    PrintPTE(console, "PDPT[0]       = ", pdpt0[0]);
    // page directory table entries (page table locations)
    PrintPDT(console, pdt0, 0, 4);
    // first 4 page table entries
    PrintPT(console, pdt0, 0, 0, 4);
}

int main(int argc, char *argv[], char *envp[])
{
    dump_page_tables();
    fputs("connect to serial 0 (COM1) for the console\n", console);
    InstallISRs(); // install keyboard handler
    ShowCPUID();
    Menu();
    return 0;
}
