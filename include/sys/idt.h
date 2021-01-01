#ifndef SYS_IDT_H_INCLUDED
#define SYS_IDT_H_INCLUDED

#include <stdint.h>

#if __x86_64__
// IDT entry descriptor
struct IDT_entry {
    uint16_t    offset_lo;  // offset bits 0..15
    uint16_t    selector;   // a code segment selector in GDT or LDT
    uint8_t     ist;        // Interrupt Stack Table offset
    uint8_t     type_attr;  // type and attributes
    uint16_t    offset_hi;  // offset bits 16..31
    uint32_t    offset_hihi;// offset bits 32..63
    uint32_t    zero;       // unused, set to zero
} __attribute__((packed));

// descriptor for the entire IDT
struct IDTR {
    uint16_t    limit;      // size in bytes - 1
    uint64_t    offset;     // pointer to IDT
} __attribute__((packed));
#else
// IDT entry descriptor
struct IDT_entry {
    uint16_t    offset_lo;  // offset bits 0..15
    uint16_t    selector;   // a code segment selector in GDT or LDT
    uint8_t     zero;       // unused, set to zero
    uint8_t     type_attr;  // type and attributes
    uint16_t    offset_hi;  // offset bits 16..31
} __attribute__((packed));

// descriptor for the entire IDT
struct IDTR {
    uint16_t    limit;      // size in bytes - 1
    uint32_t    offset;     // pointer to IDT
} __attribute__((packed));
#endif

static inline void LoadIDT(struct IDTR *idtr)
{
    __asm__ __volatile__ (
        "lidt %0\n\t"
        "sti"
        :
        : "m"(*idtr)
    );
}

void IDT_IntGate(struct IDT_entry *d, void (*hndlr)(void), int sel, int dpl);
void IDT_TrapGate(struct IDT_entry *d, void (*hndlr)(void), int sel, int dpl);

#define IDT_ADDRESS 0x1000
#define IDTR_ADDRESS 0x2000

#endif /* SYS_IDT_H_INCLUDED */
