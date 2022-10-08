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
    const unsigned int padding  = 50;
    const unsigned int screen_w = Window_Width() - padding;
    const unsigned int screen_h = Window_Height() - padding;

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        // 0 - Circle, 1 - Box
        const int shape_type = random_int(0, 1);

        const float x = (float)random_int(padding, screen_w);
        const float y = (float)random_int(padding, screen_h);

        if (shape_type == SHAPE_CIRCLE)
        {
            world.bodies[i] = Rigid_Circle_Init((vec2){x, y},
                                                20.0f,
                                                2.0f,
                                                0.5f,
                                                false);
        }
        else if (shape_type == SHAPE_BOX)
        {
            world.bodies[i] = Rigid_Box_Init((vec2){x, y},
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

        world.body_colour[i] = random_uint();
    }
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

        body->position.x += (body->linear_velocity.x * (float)elapsed_time_second);
        body->position.y += (body->linear_velocity.y * (float)elapsed_time_second);

        body->rotation += (body->rotational_velocity * (float)elapsed_time_second);
    }

    // TRANSFORM
    // const float angle = (float)(M_PI_2 * elapsed_time_second);
    float angle = 0.0f;
    if (input_key_is_down(KEY_R))
    {
        angle = (float)(M_PI_2 * elapsed_time_second);
    }

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &world.bodies[i];

        if (body->type != SHAPE_BOX)
            continue;

        vec2 *verts = body->verticies;

        verts[0] = vec2_transform(verts[0], angle);
        verts[1] = vec2_transform(verts[1], angle);
        verts[2] = vec2_transform(verts[2], angle);
        verts[3] = vec2_transform(verts[3], angle);

        body->transformed_verticies[0] = vec2_add(verts[0], body->position);
        body->transformed_verticies[1] = vec2_add(verts[1], body->position);
        body->transformed_verticies[2] = vec2_add(verts[2], body->position);
        body->transformed_verticies[3] = vec2_add(verts[3], body->position);
    }

    // COLLISIONS
    for (size_t i = 0; i < NUM_OF_BODIES - 1; i++)
    {
        Rigid *body1 = &world.bodies[i];

        for (size_t j = i + 1; j < NUM_OF_BODIES; j++)
        {
            Rigid *body2 = &world.bodies[j];

            vec2  normal = {0};
            float depth  = 0.0f;

            if (collision_test(*body1, *body2, &normal, &depth))
            {
                fprintf(stdout, "Collision between : %d and %d\n", body1->type, body2->type);

                const vec2 move_amount = vec2_mul_scal(normal, depth / 2.0f);

                Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
                Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
            }
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
    Window_Draw_Circle((int)world.bodies[0].radius, (int)world.bodies[0].position.x, (int)world.bodies[0].position.y, 0x330011);

    for (size_t i = 1; i < NUM_OF_BODIES; i++)
    {
        const Rigid        body   = world.bodies[i];
        const unsigned int colour = world.body_colour[i];

        if (body.type == SHAPE_CIRCLE)
        {
            Window_Draw_Circle((int)body.radius, (int)body.position.x, (int)body.position.y, colour);
            // Window_Draw_Circle_Outline(body.radius, body.position.x, body.position.y, colour);
        }
        else if (body.type == SHAPE_BOX)
        {
            // Window_Draw_Rectangle((int)body.position.x, (int)body.position.y, (int)body.width, (int)body.height, colour);
            const unsigned int *ind   = body.indicies;
            vec2               *verts = body.transformed_verticies;

            const vec2 v0 = verts[0];
            const vec2 v1 = verts[1];
            const vec2 v2 = verts[2];
            const vec2 v3 = verts[3];

            Window_Draw_Triangle(v0, v1, v2, colour);
            Window_Draw_Triangle(v0, v2, v3, colour);
        }
    }
}

void World_on_exit(void)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid_Destroy(&world.bodies[i]);
    }
}
