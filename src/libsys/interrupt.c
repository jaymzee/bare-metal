#include <sys/io.h>
#include <sys/interrupt.h>
#include <sys/pic.h>

#define RTC             0x70    // CMOS/Real Time Clock/disable NMI

// on older machines its necessary to give the PIC some time to react
// to commands as they might not be processed quickly
#define wait() __asm__ __volatile__ ("nop")

// Intel 8259 interrupt controller

/* reinitialize the PIC controllers, giving them new offsets
   rather than 8h and 70h, as configured by the BIOS
    offset1 - vector offset for master PIC
              vectors on the master become offset1..offset1+7
    offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_OffsetIRQ(int offset1, int offset2)
{
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA);        // save masks
    a2 = inb(PIC2_DATA);

    // start the initialization sequence (in cascade mode)
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    wait();
    outb(PIC1_DATA, offset1);   // ICW2: Master PIC vector offset
    wait();
    outb(PIC2_DATA, offset2);   // ICW2: Slave PIC vector offset
    wait();
    outb(PIC1_DATA, 4);         // ICW3: tell Master PIC that there is a slave
    wait();                     //       PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 2);         // ICW3: tell Slave PIC its cascade identity
    wait();                     //       (0000 0010)
    outb(PIC1_DATA, ICW4_8086);
    wait();
    outb(PIC2_DATA, ICW4_8086);
    wait();

    outb(PIC1_DATA, a1);        // restore saved masks.
    outb(PIC2_DATA, a2);
}

// issue non-specific EOI command using OCW2
void PIC_SendEOI(unsigned char irq)
{
    if (irq >= 8) {
        outb(PIC2_CMD, OCW2_EOI);
    }
    outb(PIC1_CMD, OCW2_EOI);
}

// mask IRQ bit using OCW1
void PIC_MaskIRQ(unsigned char irq) {
    unsigned short port;
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

// unmask IRQ bit using OCW1
void PIC_UnmaskIRQ(unsigned char irq) {
    unsigned short port;
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void PIC_MaskAllIRQ(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

// helper function
static unsigned short PIC_IssueOCW3(unsigned char ocw3)
{
    // OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
    // represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain
    outb(PIC1_CMD, ocw3);
    outb(PIC2_CMD, ocw3);
    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

// Returns the combined value of the cascaded PICs irq request register
unsigned short PIC_GetIRR(void)
{
    return PIC_IssueOCW3(OCW3_READ_IRR);
}

// Returns the combined value of the cascaded PICs in service register
unsigned short PIC_GetISR(void)
{
    return PIC_IssueOCW3(OCW3_READ_ISR);
}

// unmask NMI via CMOS/RTC
void NMI_Enable(void)
{
    outb(RTC, inb(RTC) & 0x7F);
}

// mask NMI via CMOS/RTC
void NMI_Disable(void)
{
     outb(RTC, inb(RTC) | 0x80);
}

