#include "sound.h"
#include "sb16.h"

void play_sound(uint8_t* buffer, uint32_t size, uint16_t freq)
{
    sb16_play(buffer, size, freq);
}
