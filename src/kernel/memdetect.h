#pragma once
#include "../libs/boot/bootparams.h"

#define MAX_MEMORY_REGIONS 32

extern MemoryRegion g_MemRegions[MAX_MEMORY_REGIONS];
extern int g_MemRegionCount;

void memdetect_init(MemoryRegion* map, int count);
void memdetect_print();
