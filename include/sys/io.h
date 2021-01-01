static inline void
outb(unsigned short port, unsigned char value)
{
    __asm__ __volatile__ (
        "outb %b0, %w1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline unsigned char
inb(unsigned short port)
{
    unsigned char value;
    __asm__ __volatile__ (
        "inb %w1, %0"
        : "=a"(value)
        : "Nd"(port)
    );
    return value;
}
