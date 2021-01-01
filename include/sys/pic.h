/*
 * intel 8259 interrupt controller
 */

#ifndef SYS_PIC_H_INCLUDED
#define SYS_PIC_H_INCLUDED

#define PIC1            0x20        // PIC controller 1 base address
#define PIC2            0xA0        // PIC controller 2 base address
#define PIC1_CMD        PIC1        // PIC controller 1 command port
#define PIC1_DATA       (PIC1 + 1)  // PIC controller 1 data port
#define PIC2_CMD        PIC2        // PIC controller 2 command port
#define PIC2_DATA       (PIC2 + 1)  // PIC controller 2 data port

#define OCW2_EOI        0x20    // OCW2 End-of-interrupt command code
#define OCW3_READ_IRR   0x0a    // OCW3 irq ready next CMD read
#define OCW3_READ_ISR   0x0b    // OCW3 irq service next CMD read

#define ICW1_ICW4       0x01    // ICW4 (not) needed
#define ICW1_SINGLE     0x02    // Single (cascade) mode
#define ICW1_INTERVAL4  0x04    // Call address interval 4 (8)
#define ICW1_LEVEL      0x08    // Level triggered (edge) mode
#define ICW1_INIT       0x10    // Initialization - required!

#define ICW4_8086       0x01    // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02    // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08    // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C    // Buffered mode/master
#define ICW4_SFNM       0x10    // Special fully nested (not)


/* reinitialize the PIC controllers, giving them new offsets
   rather than 8h and 70h, as configured by the BIOS

   arguments:
     offset1 - vector offset for master PIC
               vectors on the master become offset1..offset1+7
     offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_OffsetIRQ(int offset1, int offset2);

// mask interrupt (disable IRQ line)
void PIC_MaskIRQ(unsigned char irq);
// mask all interrupts (disable IRQ lines on both PICs)
void PIC_MaskAllIRQ(void);
// unmask interrupt (enable IRQ line)
void PIC_UnmaskIRQ(unsigned char irq);
// issue nonspecific EOI
void PIC_SendEOI(unsigned char irq);
// Returns the combined value of the cascaded PICs irq request register
unsigned short PIC_GetIRR(void);
// Returns the combined value of the cascaded PICs in service register
unsigned short PIC_GetISR(void);

#endif
