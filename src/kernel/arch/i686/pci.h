#pragma once
#include <stdint.h>

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
} ahci_device_t;

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint8_t subclass;
    uint16_t vendor;
    uint16_t device;
} gpu_device_t;

extern gpu_device_t gpu_dev;
extern int gpu_found;

extern ahci_device_t ahci_dev;
extern int ahci_found;

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_get_bar0(uint8_t bus, uint8_t slot, uint8_t func);

void pci_scan();
