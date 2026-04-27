#pragma once
#include <stdint.h>

typedef struct {
    uint32_t* addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
} framebuffer_t;

void fb_init(framebuffer_t* fb);
void fb_putpixel(int x, int y, uint32_t color);
void fb_clear(uint32_t color);
void fb_rect(int x, int y, int w, int h, uint32_t color);
