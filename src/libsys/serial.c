/*
 * 8250 original IBM PC UART (limited to 9600 bps)
 * 16450 same arch but faster bps (dos okay at 9600 but not a multitasking OS)
 * 16550 has a 16 byte FIFO
 * 16750 has a 64 byte FIFO
 */

#include <sys/io.h>
#include <sys/serial.h>

#define COM1_PORT 0x3f8      // COM1

void COM_Init(void) {
   outb(COM1_PORT + 1, 0x00);   // Disable all interrupts
   outb(COM1_PORT + 3, 0x80);   // Enable DLAB (set baud rate divisor)
   outb(COM1_PORT + 0, 12);     // Set divisor to 12 (lo byte) 9600 baud
   outb(COM1_PORT + 1, 0);      //                   (hi byte)
   outb(COM1_PORT + 3, 0x03);   // 8 bits, no parity, one stop bit
   outb(COM1_PORT + 2, 0xC7);   // Enable FIFO, clear them, with 14-byte thresh
   outb(COM1_PORT + 4, 0x0B);   // AuxOut2 (IRQ) enabled, RTS/DTR set
}

// is serial transmitter empty
int COM_IsTxEmpty(void)
{
    return inb(COM1_PORT + 5) & 0x20;
}

// blocking
static void COM_SendChar(char a)
{
    while (!COM_IsTxEmpty())
       ;

    outb(COM1_PORT, a);
}

// blocking
void COM_WriteChar(char a)
{
    if (a == 10) {
        COM_SendChar(13);
    }
    COM_SendChar(a);
}

// does serial receive have data
int COM_IsRxReady(void)
{
    return inb(COM1_PORT + 5) & 1;
}

// blocking
char COM_ReadChar(void)
{
    while (!COM_IsRxReady())
        ;

    return inb(COM1_PORT);
}

void COM_WriteString(const char *str)
{
    for (char ch = *str; ch; ch = *++str) {
        COM_WriteChar(ch);
    }
}
