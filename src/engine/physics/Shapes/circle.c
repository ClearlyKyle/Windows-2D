#include "../Rigid.h"

Rigid Rigid_Circle_Init(vec2 position, float radius, float density, float restitution, bool is_static)
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

        .force = {0},

        .is_static = is_static,
        .area      = area,
        .mass      = mass,
        .inv_mass  = 1.0f / mass,

        .verticies             = NULL,
        .indicies              = NULL,
        .transformed_verticies = NULL,
        .vert_count            = 0,

        .type = SHAPE_CIRCLE,
    };
}
