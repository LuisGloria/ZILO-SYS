#pragma once

typedef enum {
    ZEDIT_START,
    ZEDIT_CREATE_MEMBER_ASK,
    ZEDIT_CREATE_MEMBER_NAME,
    ZEDIT_DONE
} zedit_state_t;

int zedit_run(const char* filename);

void zedit_tick(void);
