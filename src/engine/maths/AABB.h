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

static inline int cmp_AABB(const void *a, const void *b)
{
    // Sort on minimum value along either x, y, or z
    float minA = (*(AABB **)a)->min.x;
    float minB = (*(AABB **)b)->min.x;

    if (minA < minB)
        return -1;
    if (minA > minB)
        return 1;

    return 0;
}

static int sorting_axis = 0;

inline void SortAndSweepAABBArray(AABB **AABB_Array, const size_t count)
{
    // Sort the array on currently selected sorting axis (gSortAxis)
    qsort(AABB_Array, count, sizeof(AABB *), cmp_AABB);

    // Sweep the array for collisions
    float sum1[2] = {0.0f, 0.0f};
    float sum2[2] = {0.0f, 0.0f};

    for (int i = 0; i < count; i++)
    {
        // Determine AABB center point
        float p[2] = {
            0.5f * (AABB_Array[i]->min.x + AABB_Array[i]->max.x),
            0.5f * (AABB_Array[i]->min.y + AABB_Array[i]->max.y),
        };
        // Update sum and sum2 for computing variance of AABB centers
        sum1[0] += p[0];
        sum1[1] += p[1];

        sum2[0] += p[0] * p[0];
        sum2[1] += p[1] * p[1];

        // Test collisions against all possible overlapping AABBs following current one
        for (int j = i + 1; j < count; j++)
        {
            // Stop when tested AABBs are beyond the end of current AABB
            if (AABB_Array[j]->min.x > AABB_Array[i]->max.x)
                break;
            if (AABB_Test_Overlap(AABB_Array[i], AABB_Array[j]))
                TestCollision(AABB_Array[i], AABB_Array[j]);
        }
    }

    float variance[2];
    variance[0] = sum2[0] - sum1[0] * sum1[0] / count;
    variance[1] = sum2[1] - sum1[1] * sum1[1] / count;

    // Update axis sorted to be the one with greatest AABB variance
    sorting_axis = 0;
    if (variance[1] > variance[0])
        sorting_axis = 1;
}

#endif // __AABB_H__