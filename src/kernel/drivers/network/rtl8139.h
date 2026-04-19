#pragma once
#include <stdint.h>

int rtl8139_init(uint8_t bus, uint8_t slot);
void rtl8139_send(void* data, uint32_t len);
void rtl8139_get_mac(uint8_t out[6]);
