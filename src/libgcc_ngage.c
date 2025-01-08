// Count Leading Zeros (CLZ).
unsigned int __clzsi2(unsigned int x)
{
    if (x == 0)
    {
        return 32;
    }
    unsigned int n = 0;

    if (x <= 0x0000FFFF)
    {
        n += 16;
        x <<= 16;
    }
    if (x <= 0x00FFFFFF)
    {
        n += 8;
        x <<= 8;
    }
    if (x <= 0x0FFFFFFF)
    {
        n += 4;
        x <<= 4;
    }
    if (x <= 0x3FFFFFFF)
    {
        n += 2;
        x <<= 2;
    }
    if (x <= 0x7FFFFFFF)
    {
        n += 1;
    }
    return n;
}

// Convert unsigned 64-bit int to double.
double __floatundidf(unsigned long long x)
{
    if (x == 0) return 0.0;

    int shift = 0;
    while ((x & 0x8000000000000000ULL) == 0)
    {
        x <<= 1;
        shift++;
    }

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

    int shift = 0;
    while ((x & 0x80000000U) == 0)
    {
        x <<= 1;
        shift++;
    }

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

// Convert unsigned int to float.
float __floatunsisf(unsigned int x)
{
    if (x == 0) return 0.0f;

    int shift = 0;
    while ((x & 0x80000000U) == 0)
    {
        x <<= 1;
        shift++;
    }

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