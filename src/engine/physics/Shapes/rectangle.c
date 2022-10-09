#include "../Rigid.h"

static vec2 *_Box_Create_Verticies(const float width, const float height)
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

Rigid Rigid_Box_Init(vec2 position, float width, float height, float density, float restitution, bool is_static)
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

        .force = {0},

        .is_static = is_static,
        .area      = area,
        .mass      = mass,

        .indicies              = indicies,
        .verticies             = _Box_Create_Verticies(width, height),
        .transformed_verticies = calloc(4, sizeof(vec2)),
        .vert_count            = 4,

        .type = SHAPE_BOX,
    };
}