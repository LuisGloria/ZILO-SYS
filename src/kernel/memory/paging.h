#pragma once
#include <stdint.h>

#define PAGE_SIZE 4096

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2
#define PAGE_USER    0x4

typedef uint32_t page_t;
typedef uint32_t page_directory_t;

void paging_init();
void enable_paging();
void switch_page_directory(page_directory_t* dir);

page_directory_t* create_page_directory();
void map_page(page_directory_t* dir, uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
