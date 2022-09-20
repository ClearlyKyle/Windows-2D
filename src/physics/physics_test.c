#include "physics_test.h"

#include "Rigid.h"

#define NUM_OF_BODIES 10

typedef struct Scene
{
    Rigid bodies[NUM_OF_BODIES];
} Scene;

static Scene scene = {0};

void Physics_init(void)
{
    const unsigned int padding  = 20;
    const unsigned int screen_w = Window_Width() - padding;
    const unsigned int screen_h = Window_Height() - padding;

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        // 0 - Circle
        // 1 - Box
        const int shape_type = random_int(0, 1);

        const float x = (float)random_int(padding, screen_w);
        const float y = (float)random_int(padding, screen_h);

        if (shape_type == SHAPE_CIRCLE)
        {
            scene.bodies[i] = Rigid_Circle_Init((vec3){x, y, 0.0},
                                                20.0f,
                                                2.0f,
                                                0.5f,
                                                false);
        }
        else if (shape_type == SHAPE_BOX)
        {
            scene.bodies[i] = Rigid_Box_Init((vec3){x, y, 0.0},
                                             40.0f,
                                             40.0f,
                                             0.5f,
                                             0.5f,
                                             false);
        }
        else
        {
            fprintf(stdout, "Smething wrong with random shape - %d\n", shape_type);
        }
    }
}

void Physics_update(const double elapsed_time)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &scene.bodies[i];

        body->position.x += 0.1f;

        body->position.x = wrap_float(20.0f, (float)Window_Width() - 20.0f, body->position.x);
    }
}

void Physics_on_render(void)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid body = scene.bodies[i];

        if (body.type == SHAPE_CIRCLE)
        {
            Window_Draw_Circle((int)body.radius, (int)body.position.x, (int)body.position.y, 0x330011);
        }
        else if (body.type == SHAPE_BOX)
        {
            Window_Draw_Rectangle((int)body.position.x, (int)body.position.y, (int)body.width, (int)body.height, 0x003311);
        }
    }
}

void Physics_on_exit(void)
{
}
