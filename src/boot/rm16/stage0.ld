ENTRY(_start)
SECTIONS
{
    . = 0x7C00;
    .sig : AT(0x7DFE)
    {
        SHORT(0xaa55);
    }
}
