#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "../Rigid.h"

#include <stdbool.h>
#include <assert.h>

bool collision_circle_circle(const Rigid circle1, const Rigid circle2, vec2 *normal, float *depth);
bool collision_box_box(const Rigid box1, const Rigid box2, vec2 *normal, float *depth);
bool collision_box_circle(const Rigid box, const Rigid circle, vec2 *normal, float *depth);

inline bool collision_test(const Rigid body1, const Rigid body2, vec2 *normal, float *depth)
{
    const enum Shape_Type body1_type = body1.type;
    const enum Shape_Type body2_type = body2.type;

    *normal = (vec2){0.0f, 0.0f};
    *depth  = 0.0f;

    if (body1_type == SHAPE_BOX)
    {
        if (body2_type == SHAPE_BOX)
        {
            return collision_box_box(body1, body2, normal, depth);
        }
        else if (body2_type == SHAPE_CIRCLE)
        {
            return collision_box_circle(body1, body2, normal, depth);
        }
    }
    if (body1_type == SHAPE_CIRCLE)
    {
        if (body2_type == SHAPE_BOX)
        {
            return collision_box_circle(body2, body1, normal, depth);
        }
        else if (body2_type == SHAPE_CIRCLE)
        {
            return collision_circle_circle(body1, body2, normal, depth);
        }
    }

    fprintf(stderr, "No collision test for types : %d, %d\n", body1_type, body2_type);
    return false;
}

#endif // __COLLISIONS_H__