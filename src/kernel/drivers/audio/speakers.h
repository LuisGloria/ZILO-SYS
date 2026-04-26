#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>

void speaker_on(uint32_t freq);
void speaker_off();
void speaker_beep(uint32_t freq, uint32_t duration);

#endif
