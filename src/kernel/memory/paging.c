#include "paging.h"
#include "../memory.h"
#include "../heap.h"
#include "../text.h"

extern void load_cr3(uint32_t);

page_directory_t* create_page_directory()
{
    page_directory_t* dir = (page_directory_t*)kmalloc(1024 * sizeof(uint32_t));

    for (int i = 0; i < 1024; i++)
        dir[i] = 0;

    return dir;
}

void map_page(page_directory_t* dir,
              uint32_t virtual_addr,
              uint32_t physical_addr,
              uint32_t flags)
{
    uint32_t index = virtual_addr >> 22; // crude 4MB paging shortcut

    dir[index] = (physical_addr & 0xFFFFF000) | flags;
}

void switch_page_directory(page_directory_t* dir)
{
    load_cr3((uint32_t)dir);
}

page_directory_t* kernel_directory;

void paging_init()
{
    text_print("[{0x08}PAGING{0x0F}]      Paging Initialized\n");
    kernel_directory = create_page_directory();

    // identity map kernel memory (placeholder)
    for (uint32_t i = 0; i < 0x100000; i += PAGE_SIZE)
    {
        map_page(kernel_directory, i, i, PAGE_PRESENT | PAGE_WRITE);
    }

    switch_page_directory(kernel_directory);
    enable_paging();
}
