#ifndef ZMEM_H
#define ZMEM_H

#include "ztypes.h"

void *zmemset(void *dst, int v, size_t n);
void *zmemcpy(void *dst, const void *src, size_t n);

#endif
