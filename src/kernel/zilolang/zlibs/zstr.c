#include "zstr.h"

int zstrcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const u8*)a - *(const u8*)b;
}

void zstrcpy(char *dst, const char *src) {
    while (*src)
        *dst++ = *src++;
    *dst = 0;
}

size_t zstrlen(const char *s) {
    size_t n = 0;
    while (*s++) n++;
    return n;
}
