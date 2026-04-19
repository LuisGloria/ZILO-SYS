#include "arp.h"
#include <stdint.h>

typedef struct {
    uint32_t ip;
    uint8_t mac[6];
    int used;
} arp_entry_t;

static arp_entry_t cache[16];

void arp_cache_add(uint32_t ip, uint8_t mac[6])
{
    // overwrite if exists
    for (int i = 0; i < 16; i++)
    {
        if (cache[i].used && cache[i].ip == ip)
        {
            for (int j = 0; j < 6; j++)
                cache[i].mac[j] = mac[j];
            return;
        }
    }

    // find empty slot
    for (int i = 0; i < 16; i++)
    {
        if (!cache[i].used)
        {
            cache[i].used = 1;
            cache[i].ip = ip;

            for (int j = 0; j < 6; j++)
                cache[i].mac[j] = mac[j];

            return;
        }
    }

    // simple overwrite
    cache[0].used = 1;
    cache[0].ip = ip;

    for (int j = 0; j < 6; j++)
        cache[0].mac[j] = mac[j];
}

void arp_cache_init(void)
{
    for (int i = 0; i < 16; i++)
        cache[i].used = 0;
}

int arp_lookup(uint32_t ip, uint8_t mac_out[6])
{
    for (int i = 0; i < 16; i++)
    {
        if (cache[i].used && cache[i].ip == ip)
        {
            for (int j = 0; j < 6; j++)
                mac_out[j] = cache[i].mac[j];
            return 1;
        }
    }

    return 0;
}
