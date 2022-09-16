#ifndef __VEC2_H__
#define __VEC2_H__

typedef struct vec2
{
    float x, y;
} vec2;

#define VEC2_INIT_ZERO \
    (vec2) { 0.0f, 0.0f }
#define VEC2_INIT_VALUE(VAL) \
    (vec2) { (VAL), (VAL) }

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

inline vec2 vec2_div_scal(const vec2 v1, const float val)
{
    return (vec2){v1.x / val, v1.y / val};
}

#endif // __VEC2_H__