#pragma once
#include <stdint.h>

void arp_init(void);

void arp_request(uint32_t ip);
void arp_handle_packet(uint8_t* packet);

int arp_lookup(uint32_t ip, uint8_t mac_out[6]);
void arp_cache_add(uint32_t ip, uint8_t mac[6]);
