#include "fb.h"

static framebuffer_t g_fb;

void fb_init(framebuffer_t* fb)
{
    g_fb = *fb;
}

void fb_putpixel(int x, int y, uint32_t color)
{
    if (x >= g_fb.width || y >= g_fb.height)
        return;

    uint32_t* pixel = (uint32_t*)((uint8_t*)g_fb.addr + y * g_fb.pitch + x * 4);
    *pixel = color;
}

void fb_clear(uint32_t color)
{
    for (uint32_t y = 0; y < g_fb.height; y++)
    {
        for (uint32_t x = 0; x < g_fb.width; x++)
        {
            fb_putpixel(x, y, color);
        }
    }
}
