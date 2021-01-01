#include <crt0.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cpu.h>
#include <sys/interrupt.h>
#include <sys/serial.h>
#include <sys/timer.h>
#include "cpuexchndlrs.h"

#define CS 0x08
#define DPL 0

static char *args[1] = {"program"};
static char *envp[2] = {NULL, NULL};

void _init_IDT(void);

void _start(void)
{
    COM_Init();
    SetIntervalTimer(60);
#ifdef __x86_64__
    _init_IDT(); // install CPU Exception handlers
#endif
    main(1, args, envp);
}

#ifdef __x86_64__

void SystemTimerHandler(void);

// this sets up the interrupt descriptor tables then enables interrupts
void _init_IDT(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDTR *idtr = (void *)(IDTR_ADDRESS);

    // move IRQs to interrupts 0x20-2F
    // so they do not conflict with CPU exceptions
    PIC_OffsetIRQ(0x20, 0x28);
    PIC_MaskAllIRQ(); // mask all IRQ

    memset(idt, 0, (256 * sizeof(struct IDT_entry))); // clear entire IDT

    IDT_TrapGate(idt + EXC_DIVBYZERO, DivbyzeroHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_DEBUG, DebugHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_NMI, NMIHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_BREAKPOINT, BreakpointHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_OVERFLOW, OverflowHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_BOUND, BoundRangeHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_OPCODE, InvalidOpcodeHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_DBL_FAULT, DoubleFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_TSS, InvalidTSSHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_SEGMENT, SegNotPresentHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_STACK, StackSegFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_GPF, GPFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_PF, PageFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_X87_FLOAT, x87FPExceptHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_ALIGN, AlignCheckHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_MACHINE, MachineCheckHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_SIMD_FLOAT, SIMDFPExceptHandler, CS, DPL);

    IDT_IntGate(idt + 0x20, SystemTimerHandler, CS, DPL);

    idtr->offset = (uint64_t)idt;
    idtr->limit = 0x21 * sizeof(struct IDT_entry) - 1;

    LoadIDT(idtr); // enable cpu traps

    PIC_UnmaskIRQ(0);
}

#endif
