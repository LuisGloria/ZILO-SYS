#include "zctype.h"

int zisspace(char c) {
    return (c == ' ' || c == '\n' || c == '\t'
    || c == '\r' || c == '\f' || c == '\v');
}

int zisalpha(char c) {
    return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z');
}

int zisdigit(char c) {
    return (c >= '0' && c <= '9');
}
