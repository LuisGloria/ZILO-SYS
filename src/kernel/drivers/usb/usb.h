#pragma once
#include <stdint.h>

void usb_init(void);
void usb_detect_controller(
    uint8_t bus,
    uint8_t slot,
    uint8_t func,
    uint8_t prog_if
);
