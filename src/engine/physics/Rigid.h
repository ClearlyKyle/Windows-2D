#ifndef __RIGID_H__
#define __RIGID_H__

#include <stdio.h>
#include <stdbool.h>

#include "Settings.h"
#include "maths/maths.h"

enum Shape_Type
{
    SHAPE_CIRCLE,
    SHAPE_BOX,

    SHAPE_COUNT
};

typedef struct Rigid
{
    vec2  position;
    vec2  linear_velocity;
    float rotation;
    float rotational_velocity;

    float density;
    float mass;
    float restitution;
    float area;

    bool is_static;

    float radius;
    float width;
    float height;

    unsigned int *indicies;
    vec2         *verticies;
    vec2         *transformed_verticies;
    size_t        vert_count;

    enum Shape_Type type;

} Rigid;

Rigid Rigid_Box_Init(vec2 position, float width, float height, float density, float restitution, bool is_static);
Rigid Rigid_Circle_Init(vec2 position, float radius, float density, float restitution, bool is_static);

inline void Rigid_Move_Amount_2D(vec2 *position, const float x, const float y)
{
    position->x += x;
    position->y += y;
}

inline void Rigid_Destroy(Rigid *body)
{
    if (body->verticies != NULL)
    {
        free(body->verticies);
        body->verticies = NULL;
    }

    if (body->indicies != NULL)
    {
        free(body->indicies);
        body->indicies = NULL;
    }

    if (body->transformed_verticies != NULL)
    {
        free(body->transformed_verticies);
        body->transformed_verticies = NULL;
    }
}

#endif // __RIGID_H__