#include <sys/lfb/draw.h>

static uint32_t *framebuffer;
static uint16_t fb_xres;

#define ABS(x) ((x) >= 0 ? (x) : (-x))

void
LFB_Init(uint32_t *address, uint16_t xres) {
    framebuffer = address;
    fb_xres = xres;
}

inline void
LFB_SetPixel(uint16_t x, uint16_t y, uint32_t color)
{
    framebuffer[FBWIDTH*y+x]=color;
}

// Midpoint algorithm for 2D line
void
LFB_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int incx = x1 > x0 ? 1 : -1;
    const int incy = y1 > y0 ? 1 : -1;
    int x = x0;
    int y = y0;

    LFB_SetPixel(x, y, color);
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
            LFB_SetPixel(x, y, color);
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
            LFB_SetPixel(x, y, color);
        }
    }
}
