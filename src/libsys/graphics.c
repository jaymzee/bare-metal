#include <sys/io.h>
#include <sys/graphics.h>
#include <sys/vga.h>

#define SCREEN_WIDTH 320
#define SET_PIXEL(x,y,c) video_ram[SCREEN_WIDTH*y+x]=color

/* VGA Mode 13h drawing functions */

static unsigned char *video_ram = (unsigned char *)VGA_VIDEO_MEM;

void
SetPixel(uint32_t x, uint32_t y, uint8_t color)
{
    SET_PIXEL(x, y, color);
}

// Midpoint algorithm for 2D line
void
DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int x = x0;
    int y = y0;
    int incx = 1;
    int incy = 1;

    if (dx < 0) {
        dx = -dx;
        incx = -1;
    }

    if (dy < 0) {
        dy = -dy;
        incy = -1;
    }

    SET_PIXEL(x, y, color);
    if (dx > dy) {
        const int incrE = dy * 2;
        const int incrSE = (dy - dx) * 2;
        int d = dy * 2 - dx;
        while (x < x1) {
            if (d <= 0) {
                d += incrE;
                x += incx;
            } else {
                d += incrSE;
                x += incx;
                y += incy;
            }
            SET_PIXEL(x, y, color);
        }
    } else {
        const int incrS = dx * 2;
        const int incrSE = (dx - dy) * 2;
        int d = dx * 2 - dy;
        while (y < y1) {
            if (d <= 0) {
                d += incrS;
                y += incy;
            } else {
                d += incrSE;
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

