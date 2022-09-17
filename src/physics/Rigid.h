#ifndef __RIGID_H__
#define __RIGID_H__

#include <stdbool.h>

#include "../engine/maths/vec3.h"

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
    return (Rigid){
        .position    = position,
        .radius      = radius,
        .density     = density,
        .restitution = restitution,
        .is_static   = is_static,
    };
}

#endif // __RIGID_H__