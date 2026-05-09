#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

void mouse_init();
void mouse_handler();

int mouse_get_x();
int mouse_get_y();

static int last_x = 40;
static int last_y = 12;

void mouse_draw_cursor();

extern volatile int mouse_updated;

#endif
