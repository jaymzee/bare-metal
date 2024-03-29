#include <stdint.h>
#include <stdio.h>

enum cpu_exception {
    EXC_DIVBYZERO = 0,      // Divide-by-zero Error
    EXC_DEBUG,
    EXC_NMI,
    EXC_BREAKPOINT,
    EXC_OVERFLOW,
    EXC_BOUND,              // Bound Range Exceeded
    EXC_OPCODE,             // Invalid Opcode
    EXC_DEV_NOT_AVAIL,      // Device (FPU) Not Available
    EXC_DBL_FAULT,          // Double Fault
    EXC_COPROC_OVERRUN,     // Coprocessor Segment Overrun (not used anymore)
    EXC_TSS,                // Invalid TSS
    EXC_SEGMENT,            // Segment not present
    EXC_STACK,              // Stack Segment fault
    EXC_GPF,                // General Protection Fault
    EXC_PF,                 // Page Fault
    EXC_X87_FLOAT = 0x10,   // x87 Floating point exception
    EXC_ALIGN,              // Alignment Check
    EXC_MACHINE,            // Machine Check
    EXC_SIMD_FLOAT,         // SIMD Floating-Point exception
    EXC_VIRT,               // Virtualization Exception
    EXC_SECURITY = 0x1e,    // Security Exception
    EXC_3X_FAULT = 0x1f     // Triple Fault
};

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

void DumpPTE(char *buf, uint64_t pte, int os_bits);

extern const char *cpu_exception[32];

#if __x86_64__
struct cpu_reg {
    uint64_t rax;   // 0
    uint64_t rcx;   // 8
    uint64_t rdx;   // 16
    uint64_t rbx;   // 24
    uint64_t rsp;   // 32
    uint64_t rbp;   // 40
    uint64_t rsi;   // 48
    uint64_t rdi;   // 56
    uint64_t rflags;// 64
    uint64_t rip;   // 72
    uint64_t r8;    // 80
    uint64_t r9;    // 88
    uint64_t r10;   // 96
    uint64_t r11;   // 104
    uint64_t r12;   // 112
    uint64_t r13;   // 120
    uint64_t r14;   // 128
    uint64_t r15;   // 136
    uint64_t cr0;   // 144
    uint64_t cr2;   // 152
    uint64_t cr3;   // 160
    uint64_t cr4;   // 168
    uint64_t cr8;   // 176
    uint16_t es;    // 184
    uint16_t cs;    // 186
    uint16_t ss;    // 188
    uint16_t ds;    // 190
    uint16_t fs;    // 192
    uint16_t gs;    // 194
};
#else
struct cpu_reg {
    uint32_t eax;   // 0
    uint32_t ecx;   // 4
    uint32_t edx;   // 8
    uint32_t ebx;   // 12
    uint32_t esp;   // 16
    uint32_t ebp;   // 20
    uint32_t esi;   // 24
    uint32_t edi;   // 28
    uint32_t eflags;// 32
    uint32_t eip;   // 36
    uint32_t cr0;   // 40
    uint32_t cr2;   // 44
    uint32_t cr3;   // 48
    uint32_t cr4;   // 52
    uint16_t es;    // 56
    uint16_t cs;    // 58
    uint16_t ss;    // 60
    uint16_t ds;    // 62
    uint16_t fs;    // 64
    uint16_t gs;    // 66
};
#endif

struct cpuid_reg {
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};

union cpuid_result {
    struct cpuid_reg reg;
    char str[16];
};

// DumpCPURegister dumps the CPU registers to sbuf
//   ctrl: set to true to show cpu control registers
// make sure sbuf is large enough to hold contents
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl);

// DumpPTE dumps a page table entry to buf
void DumpPTE(char *buf, uint64_t pte, int os_bits);

// PrintPTE decodes and prints a page table entry
void PrintPTE(FILE *fp, char *prefix, uint64_t pte);
// PrintPT prints a page table (an entry in the page directory table)
void PrintPT(FILE *fp, uint64_t *pdt, int pdt_indx, int start, int stop);
// PrintPDT prints a page directory table
void PrintPDT(FILE *fp, uint64_t *pdt, int start, int stop);

// DumpMem dumps memory starting from ptr as n machine words to sbuf
// make sure sbuf is large enough to hold contents
void DumpMem(char *sbuf, void *ptr, int n);

void _cpuid(struct cpuid_reg *result, uint32_t eax);

static inline void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   __asm__ __volatile__("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
