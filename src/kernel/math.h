#include "retc.h"
#ifndef MATH_H
#define MATH_H

int math_add(int a, int b);
int math_sub(int a, int b);
int math_mul(int a, int b);
retc_t math_div(int a, int b, int* out);

#include <stdint.h>

float Q_rsqrt(float number);

// Useful in OSDEV
float sqrt_approx(float number);
float absf(float x);

#endif
