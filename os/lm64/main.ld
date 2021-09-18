ENTRY(_start);
SECTIONS
{
    . = 0x40000;
    .text :
    {
        *(.text.start);
        *(.text*);
    }
}
