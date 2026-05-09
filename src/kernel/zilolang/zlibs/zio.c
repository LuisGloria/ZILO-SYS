#include "zio.h"
#include <stdarg.h>

// output
extern void backend_putc_at(char c);

void zputc(char c) {
    backend_putc_at(c);
}

void zprint(const char *s) {
    while (*s)
        zputc(*s++);
}

void zprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            zputc(*p);
            continue;
        }

        p++;

        if (*p == 's') {
            const char *s = va_arg(args, const char*);
            zprint(s);
        }
        else if (*p == 'd') {
            int v = va_arg(args, int);

            if (v == 0) {
                zputc('0');
                continue;
            }

            if (v < 0) {
                zputc('-');
                v = -v;
            }

            char buf[16];
            int i = 0;

            while (v > 0) {
                buf[i++] = '0' + (v % 10);
                v /= 10;
            }

            while (i--)
                zputc(buf[i]);
        }
    }

    va_end(args);
}
