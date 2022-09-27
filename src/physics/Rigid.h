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
    vec3  position;
    vec3  linear_velocity;
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

    enum Shape_Type type;

} Rigid;

inline Rigid Rigid_Init(vec3  position,
                        vec3  linear_velocity,
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

inline Rigid Rigid_Circle_Init(vec3 position, float radius, float density, float restitution, bool is_static)
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

        .type = SHAPE_CIRCLE,
    };
}

inline Rigid Rigid_Box_Init(vec3 position, float width, float height, float density, float restitution, bool is_static)
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

    return (Rigid){
        .position    = position,
        .width       = width,
        .height      = height,
        .density     = density,
        .restitution = restitution,
        .is_static   = is_static,
        .area        = area,
        .mass        = mass,

        .type = SHAPE_BOX,
    };
}

inline void Rigid_Move_Amount_3D(vec3 *position, const vec3 amount)
{
    position->x += amount.x;
    position->y += amount.y;
    position->z += amount.z;
}

inline void Rigid_Move_Amount_2D(vec3 *position, const float x, const float y)
{
    position->x += x;
    position->y += y;
}

#endif // __RIGID_H__