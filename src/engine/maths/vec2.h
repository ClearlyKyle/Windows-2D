#ifndef __VEC2_H__
#define __VEC2_H__

#include <math.h>

typedef struct vec2
{
    float x, y;
} vec2;

#define VEC2_INIT_ZERO \
    (vec2) { 0.0f, 0.0f }
#define VEC2_INIT_VALUE(VAL) \
    (vec2) { (VAL), (VAL) }

#define VEC2_NEGATIVE(VEC) \
    (vec2) { -(VEC).x, -(VEC).y }

inline void vec2_make_negative(vec2 *v)
{
    v->x = -v->x;
    v->y = -v->y;
}

inline vec2 vec2_add(const vec2 v1, const vec2 v2)
{
    return (vec2){v1.x + v2.x, v1.y + v2.y};
}

inline vec2 vec2_add_scal(const vec2 v1, const float val)
{
    return (vec2){v1.x + val, v1.y + val};
}

inline vec2 vec2_sub(const vec2 v1, const vec2 v2)
{
    return (vec2){v1.x - v2.x, v1.y - v2.y};
}

inline vec2 vec2_mul(const vec2 v1, const vec2 v2)
{
    return (vec2){v1.x * v2.x, v1.y * v2.y};
}

inline vec2 vec2_mul_scal(const vec2 v1, const float val)
{
    return (vec2){v1.x * val, v1.y * val};
}

inline vec2 vec2_div(const vec2 v1, const vec2 v2)
{
    return (vec2){v1.x / v2.x, v1.y / v2.y};
}

// v1 / value;
inline vec2 vec2_div_scal(const vec2 v1, const float val)
{
    return (vec2){v1.x / val, v1.y / val};
}

inline float vec2_dot(const vec2 v1, const vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

inline float vec2_length(const vec2 v1)
{
    const vec2 tmp = vec2_mul(v1, v1);
    return sqrtf(tmp.x + tmp.y);
}

inline void vec2_normalise_r(vec2 *v1)
{
    const float length = 1.0f / vec2_length(*v1);

    v1->x *= length;
    v1->y *= length;
}

inline vec2 vec2_normalise(const vec2 v1)
{
    const float length = vec2_length(v1);
    return (vec2){v1.x / length, v1.y / length};
}

inline float vec2_distance(const vec2 v1, const vec2 v2)
{
    const float xx = v2.x - v1.x;
    const float yy = v2.y - v1.y;

    const float d = sqrtf((xx * xx) + (yy * yy));

    return d;
}

inline vec2 vec2_transform(const vec2 v, float angle)
{
    const float c = cosf(angle);
    const float s = sinf(angle);

    const float rot_x = v.x * c - v.y * s;
    const float rot_y = v.x * s + v.y * c;

    return (vec2){rot_x, rot_y};
}

#endif // __VEC2_H__