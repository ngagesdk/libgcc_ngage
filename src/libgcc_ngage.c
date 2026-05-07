// Branchless binary-search CLZ for ARM920T (ARMv4T).
// Uses MOVS with shifted-register operands and conditional execution.
static inline unsigned int clz32(unsigned int x)
{
    unsigned int n, tmp;
    __asm__ __volatile__ (
        "mov     %[n], #0\n\t"
        "movs    %[t], %[x], lsr #16\n\t"
        "addeq   %[n], %[n], #16\n\t"
        "moveq   %[x], %[x], lsl #16\n\t"
        "movs    %[t], %[x], lsr #24\n\t"
        "addeq   %[n], %[n], #8\n\t"
        "moveq   %[x], %[x], lsl #8\n\t"
        "movs    %[t], %[x], lsr #28\n\t"
        "addeq   %[n], %[n], #4\n\t"
        "moveq   %[x], %[x], lsl #4\n\t"
        "movs    %[t], %[x], lsr #30\n\t"
        "addeq   %[n], %[n], #2\n\t"
        "moveq   %[x], %[x], lsl #2\n\t"
        "movs    %[t], %[x], lsr #31\n\t"
        "addeq   %[n], %[n], #1\n\t"
        "cmp     %[x], #0\n\t"
        "moveq   %[n], #32\n\t"
        : [n] "=&r" (n), [x] "+r" (x), [t] "=&r" (tmp)
        :
        : "cc"
    );
    return n;
}

static inline unsigned int clz64(unsigned long long x)
{
    unsigned int hi = (unsigned int)(x >> 32);
    if (hi != 0) return clz32(hi);
    return 32 + clz32((unsigned int)x);
}

// Count Leading Zeros (CLZ).
unsigned int __clzsi2(unsigned int x)
{
    return clz32(x);
}

// Convert signed 64-bit int to double.
double __floatdidf(long long x)
{
    if (x == 0) return 0.0;

    unsigned long long sign = 0;
    unsigned long long u;

    if (x < 0)
    {
        sign = 0x8000000000000000ULL;
        u = (unsigned long long)(-x);
    }
    else
    {
        u = (unsigned long long)x;
    }

    int shift = (int)clz64(u);
    u <<= shift;

    unsigned long long mantissa = (u & 0x7FFFFFFFFFFFFFFFULL) >> 11;
    int exponent = 1023 + 63 - shift;

    union
    {
        double d;
        unsigned long long i;

    } result;

    result.i = sign | ((unsigned long long)exponent << 52) | mantissa;
    return result.d;
}

// Convert unsigned 64-bit int to double.
double __floatundidf(unsigned long long x)
{
    if (x == 0) return 0.0;

    int shift = (int)clz64(x);
    x <<= shift;

    unsigned long long mantissa = (x & 0x7FFFFFFFFFFFFFFFULL) >> 11;
    int exponent = 1023 + 63 - shift;

    union
    {
        double d;
        unsigned long long i;

    } result;

    result.i = ((unsigned long long)exponent << 52) | mantissa;
    return result.d;
}

// Convert unsigned int to double.
double __floatunsidf(unsigned int x)
{
    if (x == 0) return 0.0;

    int shift = (int)clz32(x);
    x <<= shift;

    unsigned long long mantissa = (unsigned long long)(x & 0x7FFFFFFFU) << 21;
    int exponent = 1023 + 31 - shift;

    union
    {
        double d;
        unsigned long long i;

    } result;

    result.i = ((unsigned long long)exponent << 52) | mantissa;
    return result.d;
}

// Convert unsigned 64-bit int to float.
float __floatundisf(unsigned long long x)
{
    if (x == 0)
    {
        return 0.0f;
    }

    int shift = (int)clz64(x);
    x <<= shift;

    unsigned long long mantissa = (x & 0x7FFFFFFFFFFFFFFFULL) >> 40;
    int exponent = 127 + 63 - shift;

    union
    {
        float f;
        unsigned int i;

    } result;

    result.i = ((unsigned int)exponent << 23) | (unsigned int)mantissa;
    return result.f;
}

// Convert unsigned int to float.
float __floatunsisf(unsigned int x)
{
    if (x == 0) return 0.0f;

    int shift = (int)clz32(x);
    x <<= shift;

    unsigned int mantissa = (x & 0x7FFFFFFFU) >> 8;
    int exponent = 127 + 31 - shift;

    union
    {
        float f;
        unsigned int i;

    } result;

    result.i = ((unsigned int)exponent << 23) | mantissa;
    return result.f;
}

// Convert float to unsigned int (truncation toward zero).
unsigned int __fixunssfsi(float x)
{
    if (x <= 0.0f) return 0;
    if (x >= 4294967296.0f) return 0xFFFFFFFFU;

    union
    {
        float f;
        unsigned int i;

    } u;

    u.f = x;

    int exponent = (int)((u.i >> 23) & 0xFF) - 127;

    if (exponent < 0) return 0;
    if (exponent > 31) return 0xFFFFFFFFU;

    unsigned int mantissa = (u.i & 0x007FFFFFU) | 0x00800000U;

    if (exponent >= 23)
    {
        return mantissa << (exponent - 23);
    }
    else
    {
        return mantissa >> (23 - exponent);
    }
}

// Convert double to unsigned int (truncation toward zero).
unsigned int __fixunsdfsi(double x)
{
    if (x <= 0.0) return 0;
    if (x >= 4294967296.0) return 0xFFFFFFFFU;

    union
    {
        double d;
        unsigned long long i;

    } u;

    u.d = x;

    int exponent = (int)((u.i >> 52) & 0x7FF) - 1023;

    if (exponent < 0) return 0;
    if (exponent > 31) return 0xFFFFFFFFU;

    unsigned long long mantissa = (u.i & 0x000FFFFFFFFFFFFFULL) | 0x0010000000000000ULL;

    if (exponent >= 52)
    {
        return (unsigned int)(mantissa << (exponent - 52));
    }
    else
    {
        return (unsigned int)(mantissa >> (52 - exponent));
    }
}

// Check unordered for double precision.
int __unorddf2(double a, double b)
{
    union { double f; unsigned long long i; } ua = { .f = a }, ub = { .f = b };
    unsigned long long abs_a = ua.i & 0x7FFFFFFFFFFFFFFF; // Clear sign bit.
    unsigned long long abs_b = ub.i & 0x7FFFFFFFFFFFFFFF; // Clear sign bit.

    if ((abs_a >= 0x7FF0000000000001) || (abs_b >= 0x7FF0000000000001))
    {
        return 1; // Unordered.
    }

    return 0; // Not unordered.
}
