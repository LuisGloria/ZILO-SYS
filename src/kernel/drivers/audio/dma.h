#pragma once
#include <stdint.h>

void dma_init();
void dma_set_channel_1(void* phys_addr, uint16_t len);
