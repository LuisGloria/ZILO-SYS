#ifndef ZSTR_H
#define ZSTR_H

#include "ztypes.h"

int zstrcmp(const char *a, const char *b);
void zstrcpy(char *dst, const char *src);
size_t zstrlen(const char *s);

#endif
