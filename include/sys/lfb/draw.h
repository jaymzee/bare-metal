#include <stdint.h>

void LFB_Init(uint32_t *fb_address, uint16_t xres);
void LFB_SetPixel(uint16_t x, uint16_t y, uint32_t color);
void LFB_DrawLine(uint16_t x0, uint16_t y0,
                  uint16_t x1, uint16_t y1, uint32_t color);
