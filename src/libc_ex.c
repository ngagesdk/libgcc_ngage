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

    unsigned int high = x >> 32;
    unsigned int low = x & 0xFFFFFFFF;
    double result = 0.0;

    // Construct double from high and low manually.
    while (high > 0)
    {
        result += 4294967296.0; // Add 2^32.
        high--;
    }

    while (low > 0)
    {
        result += 1.0; // Add 1 for each unit in low.
        low--;
    }

    return result;
}

// Convert unsigned int to double.
double __floatunsidf(unsigned int x)
{
    if (x == 0) return 0.0;
    double result = 0.0;

    // Construct double from unsigned integer manually.
    while (x > 0)
    {
        result += 1.0;
        x--;
    }

    return result;
}

// Convert unsigned int to float.
float __floatunsisf(unsigned int x)
{
    if (x == 0) return 0.0f;
    float result = 0.0f;

    // Construct float from unsigned integer manually.
    while (x > 0)
    {
        result += 1.0f;
        x--;
    }

    return result;
}

// Check unordered for double precision.
int __unorddf2(double a, double b)
{
    // Software implementation to check for NaN.
    union { double f; unsigned long long i; } ua = { .f = a }, ub = { .f = b };
    unsigned long long abs_a = ua.i & 0x7FFFFFFFFFFFFFFF; // Clear sign bit.
    unsigned long long abs_b = ub.i & 0x7FFFFFFFFFFFFFFF; // Clear sign bit.

    if ((abs_a > 0x7FF0000000000000) || (abs_b > 0x7FF0000000000000))
    {
        return 1; // Unordered.
    }

    return 0; // Not unordered.
}
