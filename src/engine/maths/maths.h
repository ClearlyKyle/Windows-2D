#ifndef __MATHS_H__
#define __MATHS_H__

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>

#include "vec2.h"
#include "vec3.h"

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