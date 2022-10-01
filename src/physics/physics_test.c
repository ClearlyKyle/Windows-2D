#include "physics_test.h"

#include "Rigid.h"
#include "Collisions.h"

#define NUM_OF_BODIES 10

// next video - 17

enum colour
{
    COL_WHITE  = 0xFFFFFF,
    COL_RED    = 0xFF0000,
    COL_GREEN  = 0x00FF00,
    COL_BLUE   = 0x0000FF,
    COL_YELLOW = 0xFFFF00,
};

typedef struct Scene
{
    Rigid        bodies[NUM_OF_BODIES];
    unsigned int body_colour[NUM_OF_BODIES];
} Scene;

static Scene scene = {0};

void Physics_init(void)
{
    const unsigned int padding  = 50;
    const unsigned int screen_w = Window_Width() - padding;
    const unsigned int screen_h = Window_Height() - padding;

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        // 0 - Circle
        // 1 - Box
        const int shape_type = random_int(0, 1);
        // const int shape_type = 1;

        const float x = (float)random_int(padding, screen_w);
        const float y = (float)random_int(padding, screen_h);

        if (shape_type == SHAPE_CIRCLE)
        {
            scene.bodies[i] = Rigid_Circle_Init((vec2){x, y},
                                                20.0f,
                                                2.0f,
                                                0.5f,
                                                false);
        }
        else if (shape_type == SHAPE_BOX)
        {
            scene.bodies[i] = Rigid_Box_Init((vec2){x, y},
                                             50.0f,
                                             50.0f,
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
        Rigid_Move_Amount_2D(&scene.bodies[0].position, velocity.x, velocity.y);
    }

    // Transform Box verts
    // const float angle = (float)(M_PI_2 * elapsed_time_second);
    const float angle = 0.0f;
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &scene.bodies[i];

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
    // Compare each object one time, and not itself
    for (size_t i = 0; i < NUM_OF_BODIES - 1; i++)
    {
        Rigid *body1                     = &scene.bodies[i];
        scene.body_colour[i]             = COL_WHITE;
        const enum Shape_Type body1_type = body1->type;

        for (size_t j = i + 1; j < NUM_OF_BODIES; j++)
        {
            Rigid *body2  = &scene.bodies[j];
            vec2   normal = {0};
            float  depth  = 0.0f;

            scene.body_colour[j] = COL_WHITE;

            const enum Shape_Type body2_type = body2->type;

            if (body1_type == SHAPE_BOX && body2_type == SHAPE_CIRCLE)
            {
                if (Box_Circle_collision(*body1, *body2, &normal, &depth))
                {
                    scene.body_colour[i] = COL_RED;
                    scene.body_colour[j] = COL_RED;

                    const vec2 move_amount = vec2_mul_scal(normal, depth / 2);

                    Rigid_Move_Amount_2D(&body1->position, move_amount.x, move_amount.y);
                    Rigid_Move_Amount_2D(&body2->position, -move_amount.x, -move_amount.y);
                }
            }
            else if (body2_type == SHAPE_BOX && body1_type == SHAPE_CIRCLE)
            {
                if (Box_Circle_collision(*body2, *body1, &normal, &depth))
                {
                    scene.body_colour[i] = COL_RED;
                    scene.body_colour[j] = COL_RED;

                    const vec2 move_amount = vec2_mul_scal(normal, depth / 2);

                    Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
                    Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
                }
            }

            // if (Box_intersection(*body1, *body2, &normal, &depth))
            //{
            //     scene.body_colour[i] = COL_RED;
            //     scene.body_colour[j] = COL_RED;

            //    const vec2 move_amount = vec2_mul_scal(normal, depth / 2);

            //    Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
            //    Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
            //}

            // if (Circle_intersection(*body1, *body2, &normal, &depth))
            //{
            //     const vec2 move_amount = vec2_mul_scal(normal, depth / 2);

            //    Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
            //    Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
            //}
        }
    }

    // Screen Wrapping
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = &scene.bodies[i];
        // Rigid_Move_Amount(&body->position, (vec3){0.0f, 0.1f, 0.0f});

        body->position.x = wrap_float(50.0f, (float)Window_Width() - 50.0f, body->position.x);
        body->position.y = wrap_float(50.0f, (float)Window_Height() - 50.0f, body->position.y);
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

        Window_Draw_Pixel((int)body.position.x, (int)body.position.y, COL_YELLOW);
    }
}

void Physics_on_exit(void)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid_Destroy(&scene.bodies[i]);
    }
}
