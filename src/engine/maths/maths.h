#ifndef __MATHS_H__
#define __MATHS_H__

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>
#include <float.h> // FLT_MAX / FLT_MIN

#include "vec2.h"
#include "vec3.h"
#include "AABB.h"

inline double clamp_d(double d, double min, double max)
{
    const double t = d < min ? min : d;
    return t > max ? max : t;
}

inline float clamp_f(float f, float min, float max)
{
    const float t = f < min ? min : f;
    return t > max ? max : t;
}

inline int random_int(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

inline unsigned int random_uint(void)
{
    unsigned int x = 0;

    // return rand() ^ rand() << 1;

    x = rand() & 0xff;
    x |= (rand() & 0xff) << 8;
    x |= (rand() & 0xff) << 16;
    x |= (rand() & 0xff) << 24;

    return x;
}

inline int wrap_int(int min, int max, int value)
{
    if (value > max)
        return min;

    if (value < min)
        return max;

    return value;
    // return (value > max) ? min : (value < min) ? max
    //                                            : value;
}

inline float wrap_float(float min, float max, float value)
{
    if (value > max)
        return min;

    if (value < min)
        return max;

    return value;
}

#endif // __MATHS_H__