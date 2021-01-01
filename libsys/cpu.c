#include <stdio.h>
#include <string.h>
#include <sys/cpu.h>
#include <sys/idt.h>

// TODO: startup code should determine this rather than hard coding
#define STACK_TOP 0x1000000
#define min(a,b) (a)<(b)?a:b

const char *cpu_exception[32] = {
    "Divide-by-zero Error",         // 0x0
    "Debug",                        // 0x1
    "Non-maskable Interrupt",       // 0x2
    "Breakpoint",                   // 0x3
    "Overflow",                     // 0x4
    "Bound Range Exceeded",         // 0x5
    "Invalid Opcode",               // 0x6
    "Device Not Available",         // 0x7
    "Double Fault",                 // 0x8
    "Coprocessor Overrun",          // 0x9
    "Invalid TSS",                  // 0xa
    "Segment Not Present",          // 0xb
    "Stack Segment Fault",          // 0xc
    "General Protection Fault",     // 0xd
    "Page Fault",                   // 0xe
    "Reserved 0x0f",                // 0xf
    "x87 Floating-Point Exception", // 0x10
    "Alignment Check",              // 0x11
    "Machine Check",                // 0x12
    "SIMD Floating-Point Exception",// 0x13
    "Virtualization Exception",     // 0x14
    "Reserved 0x15",                // 0x15
    "Reserved 0x16",                // 0x16
    "Reserved 0x17",                // 0x17
    "Reserved 0x18",                // 0x18
    "Reserved 0x19",                // 0x19
    "Reserved 0x1a",                // 0x1a
    "Reserved 0x1b",                // 0x1b
    "Reserved 0x1c",                // 0x1c
    "Reserved 0x1d",                // 0x1d
    "Security Exception"            // 0x1e
};

void
IDT_IntGate(struct IDT_entry *descr, void (*hndlr)(void), int sel, int dpl)
{
#if __x86_64__
    descr->offset_hihi = (uint64_t)hndlr >> 32;
    descr->offset_hi = ((uint64_t)hndlr >> 16) & 0xffff;
    descr->offset_lo = (uint64_t)hndlr & 0xffff;
    descr->ist = 0;
#else
    descr->offset_hi = (uint32_t)hndlr >> 16;
    descr->offset_lo = (uint32_t)hndlr & 0xffff;
#endif
    descr->selector = sel;
    descr->type_attr = (dpl << 5) | 0x8E; // 32-bit interupt gate
    descr->zero = 0;
}

void
IDT_TrapGate(struct IDT_entry *descr, void (*hndlr)(void), int sel, int dpl)
{
#if __x86_64__
    descr->offset_hihi = (uint64_t)hndlr >> 32;
    descr->offset_hi = ((uint64_t)hndlr >> 16) & 0xffff;
    descr->offset_lo = (uint64_t)hndlr & 0xffff;
    descr->ist = 0;
#else
    descr->offset_hi = (uint32_t)hndlr >> 16;
    descr->offset_lo = (uint32_t)hndlr & 0xffff;
#endif
    descr->selector = sel;
    descr->type_attr = (dpl << 5) | 0x8F; // 32-bit trap gate
    descr->zero = 0;
}

#if __x86_64__
// DumpCPURegister dumps the CPU registers to sbuf
//   ctrl = 1 to show cpu control registers
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl)
{
    char nbuf[17];

    strcpy(sbuf, " RIP: ");
    strcat(sbuf, ltoa(reg->rip, 16, 16, nbuf));
    strcat(sbuf, " RSP: ");
    strcat(sbuf, ltoa(reg->rsp, 16, 16, nbuf));
    strcat(sbuf, " FLG: ");
    strcat(sbuf, ltoa(reg->rflags, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " RAX: ");
    strcat(sbuf, ltoa(reg->rax, 16, 16, nbuf));
    strcat(sbuf, " RBX: ");
    strcat(sbuf, ltoa(reg->rbx, 16, 16, nbuf));
    strcat(sbuf, " RCX: ");
    strcat(sbuf, ltoa(reg->rcx, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " RDX: ");
    strcat(sbuf, ltoa(reg->rdx, 16, 16, nbuf));
    strcat(sbuf, " RSI: ");
    strcat(sbuf, ltoa(reg->rsi, 16, 16, nbuf));
    strcat(sbuf, " RDI: ");
    strcat(sbuf, ltoa(reg->rdi, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " RBP: ");
    strcat(sbuf, ltoa(reg->rbp, 16, 16, nbuf));
    strcat(sbuf, " R08: ");
    strcat(sbuf, ltoa(reg->r8, 16, 16, nbuf));
    strcat(sbuf, " R09: ");
    strcat(sbuf, ltoa(reg->r9, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " R10: ");
    strcat(sbuf, ltoa(reg->r10, 16, 16, nbuf));
    strcat(sbuf, " R11: ");
    strcat(sbuf, ltoa(reg->r11, 16, 16, nbuf));
    strcat(sbuf, " R12: ");
    strcat(sbuf, ltoa(reg->r12, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " R13: ");
    strcat(sbuf, ltoa(reg->r13, 16, 16, nbuf));
    strcat(sbuf, " R14: ");
    strcat(sbuf, ltoa(reg->r14, 16, 16, nbuf));
    strcat(sbuf, " R15: ");
    strcat(sbuf, ltoa(reg->r15, 16, 16, nbuf));
    strcat(sbuf, "\n");
    if (ctrl) {
        strcat(sbuf, " CR0: ");
        strcat(sbuf, ltoa(reg->cr0, 16, 16, nbuf));
        strcat(sbuf, " CR2: ");
        strcat(sbuf, ltoa(reg->cr2, 16, 16, nbuf));
        strcat(sbuf, " CR3: ");
        strcat(sbuf, ltoa(reg->cr3, 16, 16, nbuf));
        strcat(sbuf, "\n");
        strcat(sbuf, " CR4: ");
        strcat(sbuf, ltoa(reg->cr4, 16, 16, nbuf));
        strcat(sbuf, " CR8: ");
        strcat(sbuf, ltoa(reg->cr8, 16, 16, nbuf));
        strcat(sbuf, "\n");
    }
    strcat(sbuf, "  CS: ");
    strcat(sbuf, itoa(reg->cs, 16, 4, nbuf));
    strcat(sbuf, "   SS: ");
    strcat(sbuf, itoa(reg->ss, 16, 4, nbuf));
    strcat(sbuf, "   DS: ");
    strcat(sbuf, itoa(reg->ds, 16, 4, nbuf));
    strcat(sbuf, "   ES: ");
    strcat(sbuf, itoa(reg->es, 16, 4, nbuf));
    strcat(sbuf, "   FS: ");
    strcat(sbuf, itoa(reg->fs, 16, 4, nbuf));
    strcat(sbuf, "   GS: ");
    strcat(sbuf, itoa(reg->gs, 16, 4, nbuf));
    strcat(sbuf, "\n");
}

// DumpMem dumps memory starting from ptr as n machine words to sbuf
// make sure sbuf is large enough to hold contents
void DumpMem(char *sbuf, void *ptr, int n)
{
    char tmpstr[17];
    uint64_t *qwptr = ptr;
    for (int i = 0; i < n; i++, qwptr++) {
        if ((i % 3) == 0) {
            strcat(sbuf, "\n  ");
            strcat(sbuf, ltoa((long)qwptr, 16, 12, tmpstr));
            strcat(sbuf, ":");
        }
        strcat(sbuf, " ");
        strcat(sbuf, ltoa(*qwptr, 16, 16, tmpstr));
    }
    strcat(sbuf, "\n");
}

#else // 32-bit
// DumpCPURegister dumps the CPU registers to sbuf
//   ctrl = 1 to show cpu control registers
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl)
{
    char nbuf[20];

    strcpy(sbuf, " EIP: ");
    strcat(sbuf, itoa(reg->eip, 16, 8, nbuf));
    strcat(sbuf, " ESP: ");
    strcat(sbuf, itoa(reg->esp, 16, 8, nbuf));
    strcat(sbuf, " EBP: ");
    strcat(sbuf, itoa(reg->ebp, 16, 8, nbuf));
    strcat(sbuf, " FLG: ");
    strcat(sbuf, itoa(reg->eflags, 16, 8, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " EAX: ");
    strcat(sbuf, itoa(reg->eax, 16, 8, nbuf));
    strcat(sbuf, " EBX: ");
    strcat(sbuf, itoa(reg->ebx, 16, 8, nbuf));
    strcat(sbuf, " ECX: ");
    strcat(sbuf, itoa(reg->ecx, 16, 8, nbuf));
    strcat(sbuf, " EDX: ");
    strcat(sbuf, itoa(reg->edx, 16, 8, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, " ESI: ");
    strcat(sbuf, itoa(reg->esi, 16, 8, nbuf));
    strcat(sbuf, " EDI: ");
    strcat(sbuf, itoa(reg->edi, 16, 8, nbuf));
    strcat(sbuf, "\n");
    if (ctrl) {
        strcat(sbuf, " CR0: ");
        strcat(sbuf, itoa(reg->cr0, 16, 8, nbuf));
        strcat(sbuf, " CR2: ");
        strcat(sbuf, itoa(reg->cr2, 16, 8, nbuf));
        strcat(sbuf, " CR3: ");
        strcat(sbuf, itoa(reg->cr3, 16, 8, nbuf));
        strcat(sbuf, " CR4: ");
        strcat(sbuf, itoa(reg->cr4, 16, 8, nbuf));
        strcat(sbuf, "\n");
    }
    strcat(sbuf, "  CS: ");
    strcat(sbuf, itoa(reg->cs, 16, 4, nbuf));
    strcat(sbuf, " SS: ");
    strcat(sbuf, itoa(reg->ss, 16, 4, nbuf));
    strcat(sbuf, " DS: ");
    strcat(sbuf, itoa(reg->ds, 16, 4, nbuf));
    strcat(sbuf, "  ES: ");
    strcat(sbuf, itoa(reg->es, 16, 4, nbuf));
    strcat(sbuf, " FS: ");
    strcat(sbuf, itoa(reg->fs, 16, 4, nbuf));
    strcat(sbuf, " GS: ");
    strcat(sbuf, itoa(reg->gs, 16, 4, nbuf));
    strcat(sbuf, "\n");
}
#endif // 32-bit


#ifdef __x86_64__
void CPUExceptionHandler(struct cpu_reg *reg, int except, int errcode)
{
    char regs[768], stack[384], msg[81], tmpstr[17];
    char *haltmsg = "system halted.\n";

    strcpy(msg, "\nPANIC: ");
    strcat(msg, cpu_exception[except]);
    strcat(msg, ", error code: ");
    strcat(msg, itoa(errcode, 16, 4, tmpstr));
    strcat(msg, "\n");
    DumpCPURegisters(regs, reg, 1);
    strcpy(stack, "Stack:");
    DumpMem(stack, (void *)reg->rsp, min(12,(STACK_TOP-reg->rsp)/8));
    fputs(msg, console);
    fputs(regs, console);
    fputs(stack, console);
    fputs(haltmsg, console);
    fputs(msg, stdout);
    fputs(regs, stdout);
    fputs(stack, stdout);
    fputs(haltmsg, stdout);

    while (1) {
        __asm__ __volatile__ (
            "cli\n\t"
            "hlt"
        );
    }
}
#endif // 64-bit

