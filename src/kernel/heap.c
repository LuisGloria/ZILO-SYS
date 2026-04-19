#include <stdint.h>
#include <stddef.h>

#define HEAP_SIZE 1024 * 1024
static uint8_t g_heap[HEAP_SIZE];
static size_t g_offset = 0;

void* kmalloc(size_t size)
{
    if (g_offset + size >= HEAP_SIZE)
        return 0;

    void* ptr = &g_heap[g_offset];
    g_offset += size;

    return ptr;
}
