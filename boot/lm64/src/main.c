#include <stdio.h>
#include <string.h>
#include <sys/interrupt.h>
#include <sys/test.h>
#include <sys/timer.h>
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

int main(int argc, char *argv[], char *envp[])
{
    fputs("connect to serial 0 (COM1) for the console\n", console);
    InstallISRs(); // install keyboard handler
    Menu();
    return 0;
}

