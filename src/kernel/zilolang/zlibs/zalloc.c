#include "zalloc.h"

// already have alloc in heap.c ig
extern void *kmalloc(size_t size);

void *zmalloc(size_t size) {
    return kmalloc(size);
}

void zfree(void *ptr) {
    (void)ptr;
}
