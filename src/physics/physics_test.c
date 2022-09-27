#include "physics_test.h"

#include "Rigid.h"

#define NUM_OF_BODIES 10

typedef struct Scene
{
    Rigid        bodies[NUM_OF_BODIES];
    unsigned int body_colour[NUM_OF_BODIES];
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
        // const int shape_type = random_int(0, 1);
        const int shape_type = 0;

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

        scene.body_colour[i] = random_uint();
    }
}

void Physics_update(const double elapsed_time_ms)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &scene.bodies[i];

        // Rigid_Move_Amount(&body->position, (vec3){0.0f, 0.1f, 0.0f});

        body->position.x = wrap_float(20.0f, (float)Window_Width() - 20.0f, body->position.x);
        body->position.y = wrap_float(20.0f, (float)Window_Height() - 20.0f, body->position.y);
    }

    float       dx    = 0.0f;
    float       dy    = 0.0f;
    const float speed = 250.0f;

    if (input_key_is_down(KEY_W)) // UP
    {
        dy--;
    }
    if (input_key_is_down(KEY_A)) // LEFT
    {
        dx--;
    }
    if (input_key_is_down(KEY_S)) // DOWN
    {
        dy++;
    }
    if (input_key_is_down(KEY_D)) // RIGHT
    {
        dx++;
    }

    if (dx != 0.0f || dy != 0.0f)
    {
        // Move user
        vec2 direction = {dx, dy};
        direction      = vec2_normalise(direction);

        const float time = (float)(elapsed_time_ms / 1000); // ms -> s

        vec2 velocity = vec2_mul_scal(direction, speed);
        velocity      = vec2_mul_scal(velocity, time); // m/s

        // First body in the list will be user controller
        Rigid_Move_Amount_2D(&scene.bodies[0].position, velocity.x, velocity.y);
    }
}

void Physics_on_render(void)
{
    Window_Draw_Circle((int)scene.bodies[0].radius, (int)scene.bodies[0].position.x, (int)scene.bodies[0].position.y, 0x330011);

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        const Rigid        body   = scene.bodies[i];
        const unsigned int colour = scene.body_colour[i];

        if (body.type == SHAPE_CIRCLE)
        {
            // Window_Draw_Circle((int)body.radius, (int)body.position.x, (int)body.position.y, 0x330011);
            Window_Draw_Circle_Outline(body.radius, body.position.x, body.position.y, colour);
        }
        else if (body.type == SHAPE_BOX)
        {
            Window_Draw_Rectangle((int)body.position.x, (int)body.position.y, (int)body.width, (int)body.height, colour);
        }
    }
}

void Physics_on_exit(void)
{
}
