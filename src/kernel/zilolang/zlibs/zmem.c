#include "zmem.h"

void *zmemset(void *dst, int v, size_t n) {
    u8 *p = (u8*)dst;
    for (size_t i = 0; i < n; i++)
        p[i] = (u8)v;
    return dst;
}

void *zmemcpy(void *dst, const void *src, size_t n) {
    u8 *d = (u8*)dst;
    const u8 *s = (const u8*)src;

    for (size_t i = 0; i < n; i++)
        d[i] = s[i];

    return dst;
}
