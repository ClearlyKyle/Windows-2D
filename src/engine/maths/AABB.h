#ifndef __AABB_H__
#define __AABB_H__

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

#endif // __AABB_H__