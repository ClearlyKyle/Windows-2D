#include "World.h"

#define NUM_OF_BODIES 10

typedef struct World
{
    Rigid        bodies[NUM_OF_BODIES];
    unsigned int body_colour[NUM_OF_BODIES];
} World;

static World world = {0};

void World_init(void)
{
}

void World_update(const double elapsed_time_ms)
{
    const double elapsed_time_second = elapsed_time_ms / 1000.0;

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

        vec2 velocity = vec2_mul_scal(direction, speed);
        velocity      = vec2_mul_scal(velocity, (float)elapsed_time_second); // m/s

        // First body in the list will be user controller
        Rigid_Move_Amount_2D(&world.bodies[0].position, velocity.x, velocity.y);
    }

    // MOVEMENT
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &world.bodies[i];

        body->position.x = body->linear_velocity.x * elapsed_time_second;
        body->position.y = body->linear_velocity.y * elapsed_time_second;

        body->rotation = body->rotational_velocity * elapsed_time_second;
    }

    // COLLISIONS
    for (size_t i = 0; i < NUM_OF_BODIES - 1; i++)
    {
        Rigid *body1 = &world.bodies[i];

        for (size_t j = i + 1; j < NUM_OF_BODIES; j++)
        {
            Rigid *body2 = &world.bodies[j];
        }
    }

    // SCREEN WRAPPING
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &world.bodies[i];
        // Rigid_Move_Amount(&body->position, (vec3){0.0f, 0.1f, 0.0f});

        body->position.x = wrap_float(50.0f, (float)Window_Width() - 50.0f, body->position.x);
        body->position.y = wrap_float(50.0f, (float)Window_Height() - 50.0f, body->position.y);
    }
}

void World_on_render(void)
{
}

void World_on_exit(void)
{
}
