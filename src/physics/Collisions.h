#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "Rigid.h"

#include <stdbool.h>
#include <assert.h>

bool Circle_intersection(const Rigid circle1, const Rigid circle2, vec2 *normal, float *depth)
{
    assert(circle1.type == SHAPE_CIRCLE && "Circle_intersection - circle1 is not a CIRCLE\n");
    assert(circle2.type == SHAPE_CIRCLE && "Circle_intersection - circle2 is not a CIRCLE\n");

    const float distance        = vec2_distance(circle1.position, circle2.position);
    const float combined_radius = circle1.radius + circle2.radius;

    if (distance >= combined_radius)
        return false;

    *normal = vec2_sub(circle2.position, circle1.position);
    *normal = vec2_normalise(*normal);

    *depth = combined_radius - distance;

    return true;
}

inline static void _project_verticies(const vec2 *verticies, const size_t num_verts, const vec2 axis, float *min, float *max)
{
    *min = FLT_MAX;
    *max = FLT_MIN;

    for (size_t i = 0; i < num_verts; i++)
    {
        vec2        vert       = verticies[i];
        const float projection = vec2_dot(vert, axis);

        if (projection < *min)
            *min = projection;
        if (projection > *max)
            *max = projection;
    }
}

bool Box_intersection(const Rigid box1, const Rigid box2, vec2 *normal, float *depth)
{
    assert(box1.type == SHAPE_BOX && "Box_intersection - box1 is not a BOX\n");
    assert(box2.type == SHAPE_BOX && "Box_intersection - box2 is not a BOX\n");

    float min1, max1;
    float min2, max2;

    vec2  local_normal = (vec2){0.0f, 0.0f};
    float local_depth  = FLT_MAX;

    for (size_t i = 0; i < box1.vert_count; i++)
    {
        const vec2 vert1 = box1.transformed_verticies[i];
        const vec2 vert2 = box1.transformed_verticies[(i + 1) % box1.vert_count];

        const vec2 edge = vec2_sub(vert2, vert1);
        const vec2 axis = (vec2){-edge.y, edge.x};

        _project_verticies(box1.transformed_verticies, box1.vert_count, axis, &min1, &max1);
        _project_verticies(box2.transformed_verticies, box2.vert_count, axis, &min2, &max2);

        if (min1 >= max2 || min2 >= max1)
        {
            return false; // seperated
        }

        const float axis_depth = min((max2 - min1), (max1 - min2));

        if (axis_depth < local_depth)
        {
            local_depth  = axis_depth;
            local_normal = axis;
        }
    }

    for (size_t i = 0; i < box2.vert_count; i++)
    {
        const vec2 vert1 = box2.transformed_verticies[i];
        const vec2 vert2 = box2.transformed_verticies[(i + 1) % box2.vert_count];

        const vec2 edge = vec2_sub(vert2, vert1);
        const vec2 axis = (vec2){-edge.y, edge.x};

        _project_verticies(box1.transformed_verticies, box1.vert_count, axis, &min1, &max1);
        _project_verticies(box2.transformed_verticies, box2.vert_count, axis, &min2, &max2);

        if (min1 >= max2 || min2 >= max1)
        {
            return false; // seperated
        }

        const float axis_depth = min((max2 - min1), (max1 - min2));

        if (axis_depth < local_depth)
        {
            local_depth  = axis_depth;
            local_normal = axis;
        }
    }

    local_depth /= vec2_length(local_normal);
    local_normal = vec2_normalise(local_normal);

    const vec2 direction = vec2_sub(box2.position, box1.position);

    if (vec2_dot(direction, local_normal) < 0.0f)
        *normal = VEC2_NEGATIVE(local_normal);
    else
        *normal = local_normal;

    *depth = local_depth;

    return true; // touching
}

#endif // __COLLISIONS_H__