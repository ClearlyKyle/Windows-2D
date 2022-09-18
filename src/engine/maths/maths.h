#ifndef __MATHS_H__
#define __MATHS_H__

#define _USE_MATH_DEFINES
#include <math.h>

#include "vec2.h"
#include "vec3.h"

double clamp_d(double d, double min, double max)
{
    const double t = d < min ? min : d;
    return t > max ? max : t;
}

float clamp_f(float f, float min, float max)
{
    const float t = f < min ? min : f;
    return t > max ? max : t;
}

#endif // __MATHS_H__