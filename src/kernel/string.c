#include "string.h"

int strcmp(const char* a, const char* b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return *(const unsigned char*)a - *(const unsigned char*)b;
}

int strlen(const char* str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

int strncmp(const char* a, const char* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[i] != b[i] || a[i] == 0 || b[i] == 0)
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}
