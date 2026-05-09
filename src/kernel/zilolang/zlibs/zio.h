#ifndef ZIO_H
#define ZIO_H

#include "ztypes.h"

void backend_putc_at(char c);

void zputc(char c);
void zprint(const char *s);
void zprintf(const char *fmt, ...);

#endif
