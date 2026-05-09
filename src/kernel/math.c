#include "math.h"
#include "retc.h"

int math_add(int a, int b) { return a + b; }
int math_sub(int a, int b) { return a - b; }
int math_mul(int a, int b) { return a * b; }
#include "retc.h"

retc_t math_div(int a, int b, int* out)
{
    if (b == 0)
        return RETC_DIVISION_BY_ZERO;

    *out = a / b;
    return RETC_OK;
}

// safer bit reinterpretation helpers
static inline float int_to_float(uint32_t i)
{
    union {
        uint32_t i;
        float f;
    } u;
    u.i = i;
    return u.f;
}

static inline uint32_t float_to_int(float f)
{
    union {
        uint32_t i;
        float f;
    } u;
    u.f = f;
    return u.i;
}

float Q_rsqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}


float absf(float x)
{
    return (x < 0.0f) ? -x : x;
}

float sqrt_approx(float number)
{
    // uses inverse sqrt: sqrt(x) = x * rsqrt(x)
    return number * Q_rsqrt(number);
}
