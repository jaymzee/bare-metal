ENTRY(_start)
SECTIONS
{
    . = 0x7e00;
    .text :
    {
        *(.text.start);
        *(.text*);
    }
    .sig : AT(0x7FFE)
    {
        SHORT(0xaa55);
    }
}
