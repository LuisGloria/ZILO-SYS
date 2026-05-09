#pragma once

typedef enum
{
    RETC_OK = 0,
    RETC_UNKNOWN_CMD,
    RETC_UNKNOWN_ARG,
    RETC_WRONG_ARG,
    RETC_INVALID,
    RETC_DIVISION_BY_ZERO,
    RETC_IO_ERR,
    RETC_FILE_NOT_FOUND,
    RETC_DENIED,
    RETC_DNR,                   //Device not ready
    RETC_FULL,
    RETC_RO,                    //Read only
    RETC_OOM,                   //Out of mem
    RETC_SO,                    //Stack overflow
    RETC_BO,                    //Buffer overlow
    RETC_HEAP_CORRUPTION,
    RETC_NULL_POINTER,
    RETC_SEGFAULT,
    RETC_INVALID_STATE,
    RETC_WHERE_TF_IS_MY_DOODAD, //do NOT delete the doodad
    RETC_UNINITIALIZED,
    RETC_ALREADY_INITIALIZED,
    RETC_BUSY,
    RETC_TIMEOUT,
    RETC_ILLEGAL_INSTRUCTION,
    RETC_PRIVILEGE_VIOLATION,
    RETC_IQUIT,                 //use only for when system colapses and you're left with nothing but the shell (recovery, degraded mode)
    RETC_ISM,                   //use only for when driver write to mem it shouldn't
    RETC_FAIL                   //general error
} retc_t;

static inline const char* retc_str(retc_t code)
{
    switch (code)
    {
        case RETC_OK: return "OK";
        case RETC_UNKNOWN_CMD: return "UNKNOWN COMMAND";
        case RETC_UNKNOWN_ARG: return "UNKNOWN ARG";
        case RETC_WRONG_ARG: return "WRONG ARG";
        case RETC_INVALID: return "INVALID";
        case RETC_DIVISION_BY_ZERO: return "ILLEGAL DIVISION";
        case RETC_IO_ERR: return "IO ERROR";
        case RETC_FILE_NOT_FOUND: return "FILE NOT FOUND";
        case RETC_DENIED: return "NOPE";
        case RETC_DNR: return "DEVICE NOT READY";
        case RETC_RO: return "READ ONLY";
        case RETC_FULL: return "DISK FULL";
        case RETC_OOM: return "OUT OF MEMORY";
        case RETC_SO: return "STACK OVERFLOW";
        case RETC_BO: return "BUFFER OVERFLOW";
        case RETC_HEAP_CORRUPTION: return "HEAP CORRUPTION";
        case RETC_NULL_POINTER: return "NULL POINTER";
        case RETC_SEGFAULT: return "SEGMENTATION FAULT";
        case RETC_INVALID_STATE: return "INVALID STATE";
        case RETC_WHERE_TF_IS_MY_DOODAD: return "WHERE THE FUCK IS MY DOODAD?!!?!!?!";
        case RETC_UNINITIALIZED: return "UNINITIALIZED";
        case RETC_ALREADY_INITIALIZED: return "ALREADY INITIALIZED";
        case RETC_BUSY: return "IT'S BUSY";
        case RETC_TIMEOUT: return "TIMED OUT";
        case RETC_ILLEGAL_INSTRUCTION: return "ILLEGAL INSTR";
        case RETC_PRIVILEGE_VIOLATION: return "PRIVILEGE VIOLATION";
        case RETC_IQUIT: return "NOPE, I QUIT, YOU'RE ON YOUR OWN";
        case RETC_ISM: return "I SHAT MYSELF";
        case RETC_FAIL: return "FAIL";
        default: return "UNKNOWN";
    }
}
