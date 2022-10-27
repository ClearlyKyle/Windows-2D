#ifndef __AABB_H__
#define __AABB_H__

#include <stdbool.h>

typedef struct AABB
{
    vec2 min;
    vec2 max;
} AABB;

inline AABB AABB_Create(const float minX, const float minY, const float maxX, const float maxY)
{
    return (AABB){
        .min = (vec2){minX, minY},
        .max = (vec2){maxX, maxY},
    };
}

inline bool AABB_Test_Overlap(const AABB *a, const AABB *b)
{
    const float d1x = b->min.x - a->max.x;
    const float d1y = b->min.y - a->max.y;
    const float d2x = a->min.x - b->max.x;
    const float d2y = a->min.y - b->max.y;

    if (d1x > 0.0f || d1y > 0.0f)
        return false;

    if (d2x > 0.0f || d2y > 0.0f)
        return false;

    return true;
}

#endif // __AABB_H__