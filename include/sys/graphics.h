/* VGA Mode 13h drawing functions */

#include <stdint.h>

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void SetColorPalette(const struct color palette[256]);
void SetPixel(uint16_t x, uint16_t y, uint8_t color);
void DrawLine(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1, uint8_t color);

#ifdef FBVIRTADDR
void SetPixel32(uint16_t x, uint16_t y, uint32_t color);
void DrawLine32(uint16_t x0, uint16_t y0,
                uint16_t x1, uint16_t y1, uint32_t color);
#endif
