#include <sys/lfb/draw.h>

static uint32_t *framebuffer;
static uint16_t fb_xres;

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

    LFB_SetPixel(x, y, color);
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
            LFB_SetPixel(x, y, color);
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
            LFB_SetPixel(x, y, color);
        }
    }
}
