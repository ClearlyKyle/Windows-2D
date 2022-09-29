#ifndef __RIGID_H__
#define __RIGID_H__

#include <stdio.h>
#include <stdbool.h>

#include "Settings.h"
#include "../engine/maths/maths.h"

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

    enum Shape_Type type;

} Rigid;

inline Rigid Rigid_Init(vec2  position,
                        vec2  linear_velocity,
                        float rotation,
                        float rotational_velocity,
                        float density,
                        float mass,
                        float restitution,
                        float area,
                        bool  is_static,
                        float radius,
                        float width,
                        float height)
{
    return (Rigid){
        .position            = position,
        .linear_velocity     = linear_velocity,
        .rotation            = rotation,
        .rotational_velocity = rotational_velocity,

        .density     = density,
        .mass        = mass,
        .restitution = restitution,
        .area        = area,

        .is_static = is_static,

        .radius = radius,
        .width  = width,
        .height = height,
    };
}

inline Rigid Rigid_Circle_Init(vec2 position, float radius, float density, float restitution, bool is_static)
{
    const float area = radius * radius * (float)M_PI;

    if (area < MIN_BODY_SIZE || area > MAX_BODY_SIZE)
    {
        fprintf(stderr, "(area < MIN_BODY_SIZE || area > MAX_BODY_SIZE) area = %f\n", area);
        return (Rigid){0};
    }

    if (density < MIN_DENSITY || density > MAX_DENSITY)
    {
        fprintf(stderr, "(density < MIN_DENSITY || density > MAX_DENSITY) density = %f\n", area);
        return (Rigid){0};
    }

    restitution = clamp_f(restitution, 0.0f, 1.0f);

    // Mass
    const float mass = area * density;

    return (Rigid){
        .position    = position,
        .radius      = radius,
        .density     = density,
        .restitution = restitution,
        .is_static   = is_static,
        .area        = area,
        .mass        = mass,

        .verticies = NULL,
        .indicies  = NULL,

        .type = SHAPE_CIRCLE,
    };
}

inline static vec2 *_Box_Create_Verticies(const float width, const float height)
{
    const float left   = -width / 2.0f;
    const float right  = left + width;
    const float bottom = -height / 2.0f;
    const float top    = bottom + height;

    vec2 *verts = malloc(sizeof(vec2) * 4);

    // CW Winding
    verts[0] = (vec2){left, top};
    verts[1] = (vec2){right, top};
    verts[2] = (vec2){right, bottom};
    verts[3] = (vec2){left, bottom};

    return verts;
}

inline Rigid Rigid_Box_Init(vec2 position, float width, float height, float density, float restitution, bool is_static)
{
    const float area = width * height;

    if (area < MIN_BODY_SIZE || area > MAX_BODY_SIZE)
    {
        fprintf(stderr, "(area < MIN_BODY_SIZE || area > MAX_BODY_SIZE) area = %f\n", area);
        return (Rigid){0};
    }

    if (density < MIN_DENSITY || density > MAX_DENSITY)
    {
        fprintf(stderr, "(density < MIN_DENSITY || density > MAX_DENSITY) density = %f\n", area);
        return (Rigid){0};
    }

    restitution = clamp_f(restitution, 0.0f, 1.0f);

    // Mass
    const float mass = area * density;

    // Indicies
    unsigned int *indicies = malloc(sizeof(unsigned int) * 6);
    indicies[0]            = 0;
    indicies[1]            = 1;
    indicies[2]            = 2;
    indicies[3]            = 0;
    indicies[4]            = 2;
    indicies[5]            = 3;
    // indicies               = (unsigned int[]){0, 1, 2, 0, 2, 3};

    return (Rigid){
        .position    = position,
        .width       = width,
        .height      = height,
        .density     = density,
        .restitution = restitution,
        .is_static   = is_static,
        .area        = area,
        .mass        = mass,

        .indicies  = indicies,
        .verticies = _Box_Create_Verticies(width, height),

        .type = SHAPE_BOX,
    };
}

inline void Rigid_Move_Amount_3D(vec2 *position, const vec2 amount)
{
    position->x += amount.x;
    position->y += amount.y;
}

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
}

#endif // __RIGID_H__