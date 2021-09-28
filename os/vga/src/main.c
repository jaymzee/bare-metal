#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/vga/draw.h>
#include <sys/vga/reg.h>
#include <sys/serial.h>

struct color pal1[256];

void CreateColorPalette(struct color pal[256])
{
    for (int i = 0; i < 64; i++) {
        pal[i].r = i;
        pal[i].g = i;
        pal[i].b = i;
    }
    for (int i = 64; i < 128; i++) {
        pal[i].r = i & 0x3f;
        pal[i].g = 0;
        pal[i].b = 0;
    }
    for (int i = 128; i < 196; i++) {
        pal[i].r = 0;
        pal[i].g = i & 0x3f;
        pal[i].b = 0;
    }
    for (int i = 196; i < 256; i++) {
        pal[i].r = 0;
        pal[i].g = 0;
        pal[i].b = i & 0x3f;
    }
}

int main(int argc, char *argv[], char *envp[])
{
    uint32_t seed = 42;

    println("VGA graphics in long mode (64-bit) demo");
    CreateColorPalette(pal1);
    //SetColorPalette(pal1);
    for (int i = 0; i < 256; i++) {
        //DrawLine(i, 0, i, 199, i);
    }
    for (int i = 0; i < 10000; i++) {
        double rr = (double)rand_r(&seed) / (double)RAND_MAX;
        double ri = (double)rand_r(&seed) / (double)RAND_MAX;

        SetPixel(319*rr, 199*ri, 10);
    }
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
