#pragma once
#include "retc.h"

retc_t shell_execute(const char* cmd);
typedef void (*app_input_fn)(void);

void zedit_tick(void);
