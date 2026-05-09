#pragma once
#include "../process/process.h"

void scheduler_init();
void schedule();
void add_process(process_t* p);
