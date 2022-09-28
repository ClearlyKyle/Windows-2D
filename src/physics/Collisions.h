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

#endif // __COLLISIONS_H__