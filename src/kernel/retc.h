#pragma once

typedef enum
{
    RETC_OK = 0,
    RETC_UNKNOWN_CMD,
    RETC_UNKNOWN_ARG,
    RETC_INVALID,
    RETC_FAIL
} retc_t;

static inline const char* retc_str(retc_t code)
{
    switch (code)
    {
        case RETC_OK: return "OK";
        case RETC_UNKNOWN_CMD: return "UNKNOWN COMMAND";
        case RETC_UNKNOWN_ARG: return "UNKNOWN ARG";
        case RETC_INVALID: return "INVALID";
        case RETC_FAIL: return "FAIL";
        default: return "UNKNOWN";
    }
}
