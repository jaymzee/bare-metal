#include <sys/io.h>
#include <sys/vga/reg.h>
#include <sys/vga/draw.h>

#define SCREEN_WIDTH 320
#define SET_PIXEL(x,y,c) video_ram[SCREEN_WIDTH*y+x]=c
#define ABS(x) ((x) >= 0 ? (x) : (-x))

/* VGA Mode 13h drawing functions */

static unsigned char *video_ram = (unsigned char *)VGA_VIDEO_MEM;

void
SetPixel(uint16_t x, uint16_t y, uint8_t color)
{
    SET_PIXEL(x, y, color);
}

// Midpoint algorithm for 2D line
void
DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int incx = x1 > x0 ? 1 : -1;
    const int incy = y1 > y0 ? 1 : -1;
    int x = x0;
    int y = y0;

    SET_PIXEL(x, y, color);
    if (ABS(dx) > ABS(dy)) {
        const int incrE = 2 * ABS(dy);
        const int incrSE = 2 * (ABS(dy) - ABS(dx));
        int d = 2 * ABS(dy) - ABS(dx);
        while (x != x1) {
            if (d <= 0) {
                d += incrE; // or W
                x += incx;
            } else {
                d += incrSE; // or NE, NW, SW
                x += incx;
                y += incy;
            }
            SET_PIXEL(x, y, color);
        }
    } else {
        const int incrS = 2 * ABS(dx);
        const int incrSE = 2 * (ABS(dx) - ABS(dy));
        int d = 2 * ABS(dx) - ABS(dy);
        while (y != y1) {
            if (d <= 0) {
                d += incrS; // or N
                y += incy;
            } else {
                d += incrSE; // or SW, NW, NE
                x += incx;
                y += incy;
            }
            SET_PIXEL(x, y, color);
        }
    }
}

void
SetColorPalette(const struct color pal[])
{
    for (int i = 0; i < 256; i++) {
        outb(VGA_DAC_ADDR_WR, i);
        outb(VGA_DAC_DATA, pal[i].r);
        outb(VGA_DAC_DATA, pal[i].g);
        outb(VGA_DAC_DATA, pal[i].b);
    }
}
