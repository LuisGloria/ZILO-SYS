#include "string.h"

int atoi(const char* str)
{
    int i = 0;
    int sign = 1;
    int result = 0;

    // skip whitespace
    while (str[i] == ' ')
        i++;

    // sign handling
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }

    // convert
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result * sign;
}

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

char* strncpy(char* dest, const char* src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i]; i++)
        dest[i] = src[i];

    for (; i < n; i++)
        dest[i] = 0;

    return dest;
}
