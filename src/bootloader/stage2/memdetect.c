#include "x86.h"
#include "stdio.h"
#include "../../libs/boot/bootparams.h"

#define MAX_REGIONS 256

MemoryRegion g_MemRegions[MAX_REGIONS];
int g_MemRegionCount;

extern MemoryRegion g_MemRegions[];
extern int g_MemRegionCount;

void Memory_Detect(MemoryInfo* memoryInfo)
{
    E820MemoryBlock block;
    uint32_t continuation = 0;
    int ret;

    g_MemRegionCount = 0;

    ret = x86_E820GetNextBlock(&block, &continuation);

    while (ret > 0)
    {
        if (g_MemRegionCount >= MAX_REGIONS)
            break;

        g_MemRegions[g_MemRegionCount].Begin  = block.Base;
        g_MemRegions[g_MemRegionCount].Lenght = block.Length;
        g_MemRegions[g_MemRegionCount].Type   = block.Type;
        g_MemRegions[g_MemRegionCount].ACPI   = block.ACPI;

        printf("E820: base=0x%llx length=0x%llx type=0x%x\n",
               block.Base, block.Length, block.Type);

        g_MemRegionCount++;

        if (continuation == 0)
            break;

        ret = x86_E820GetNextBlock(&block, &continuation);
    }

    memoryInfo->RegionCount = g_MemRegionCount;
    memoryInfo->Regions = g_MemRegions;
}
