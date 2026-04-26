#include "memdetect.h"
#include "text.h"

MemoryRegion g_MemRegions[MAX_MEMORY_REGIONS];
int g_MemRegionCount = 0;

void memdetect_init(MemoryRegion* map, int count)
{
    if (count > MAX_MEMORY_REGIONS)
        count = MAX_MEMORY_REGIONS;

    for (int i = 0; i < count; i++)
        g_MemRegions[i] = map[i];

    g_MemRegionCount = count;

    text_print("Memory map received from bootloader\n");
    text_print("COUNT=");
    text_print_dec(count);
    text_print("\n");
}

void memdetect_print()
{
    for (int i = 0; i < g_MemRegionCount; i++)
    {
        text_print("Base: ");
        text_print_hex((uint32_t)(g_MemRegions[i].Begin >> 32));
        text_print_hex((uint32_t)(g_MemRegions[i].Begin));

        text_print(" Length: ");
        text_print_hex((uint32_t)(g_MemRegions[i].Lenght >> 32));
        text_print_hex((uint32_t)(g_MemRegions[i].Lenght));

        text_print(" Type: ");
        text_print_dec(g_MemRegions[i].Type);

        text_print("\n");
    }
}
