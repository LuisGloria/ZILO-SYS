#pragma once
#include <stdint.h>

int sb16_init();
void sb16_test_tone();
void play_tetris_theme();
void sb16_play(uint8_t* buf, uint32_t len, uint16_t hz);
