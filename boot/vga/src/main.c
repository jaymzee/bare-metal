#include <stdio.h>
#include <sys/io.h>
#include <sys/graphics.h>
#include <sys/serial.h>
#include <sys/vga.h>

void init()
{
    println("VGA graphics in long mode (64-bit) demo");
    struct color pal[256];
    for (int i = 0; i < 64; i++) {
        pal[i].r = i;
        pal[i].g = i;
        pal[i].b = i;
    }
    for (int i = 64; i < 128; i++) {
        pal[i].r = i;
        pal[i].g = 0;
        pal[i].b = 0;
    }
    for (int i = 128; i < 196; i++) {
        pal[i].r = 0;
        pal[i].g = i;
        pal[i].b = 0;
    }
    for (int i = 196; i < 256; i++) {
        pal[i].r = 0;
        pal[i].g = 0;
        pal[i].b = i;
    }
    SetColorPalette(pal);
}

int main(int argc, char *argv[], char *envp[])
{
    init();
    for (int i = 0; i < 256; i++) {
        DrawLine(i, 0, i, 199, i);
    }
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
