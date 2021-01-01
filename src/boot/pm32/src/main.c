#include <stdio.h>
#include <string.h>
#include <sys/conio.h>
#include <sys/serial.h>
#include <sys/cpu.h>
#include "intsetup.h"

extern void CauseGPFault(void);
extern void CauseDivbyzeroFault(void);

int main(int argc, char *argv[], char *envp[])
{
    char buf[80];

    DisableBlinkingText();
    ClearText(0x1F);
    fputs("32-bit protected mode entered successfully!\n", console);
    fputs("connect to serial 0 (COM1) for the console\n", console);
    fputs("bang on keyboard to show scan codes\n", console);

    COM_Init();

    println("installing handlers and enabling interrupts...");
    EnableInterrupts();
    println("32-bit protected mode demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        print("\n");
        print(itoa(timer_count, 10, 0, buf));
    }
}

void GPFaultHandlerM(const struct cpu_reg *reg, int errcode)
{
    char regs[500], mesg[80], selector[20];

    strcpy(mesg, "\nPANIC: GP Fault, selector: ");
    strcat(mesg, itoa(errcode, 16, 4, selector));
    strcat(mesg, "\n");
    DumpCPURegisters(regs, reg, 1);

    fputs(mesg, console);
    fputs(regs, console);
    fputs(mesg, stdout);
    fputs(regs, stdout);
}

void DivbyzeroHandlerM(const struct cpu_reg *reg)
{
    char regs[500];
    const char *mesg;

    mesg = "\nPANIC: div by zero\n";
    DumpCPURegisters(regs, reg, 1);

    fputs(mesg, console);
    fputs(regs, console);
    fputs(mesg, stdout);
    fputs(regs, stdout);
}
