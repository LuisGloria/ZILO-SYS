#pragma once
#include <stdint.h>

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_get_bar0(uint8_t bus, uint8_t slot, uint8_t func);

void pci_scan();
