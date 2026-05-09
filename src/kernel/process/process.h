#pragma once
#include <stdint.h>
#include "../arch/i686/isr.h"
#include "../memory/paging.h"

typedef enum {
    PROC_READY,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_TERMINATED
} process_state_t;

typedef struct process {
    int pid;

    page_directory_t* page_dir;

    Registers regs;

    uint32_t kernel_stack;
    uint32_t user_stack;

    process_state_t state;

    struct process* next;
} process_t;

process_t* create_process();
