#include "World.h"

#define NUM_OF_BODIES 10

// 13

typedef struct World
{
    Rigid       *bodies[NUM_OF_BODIES];
    unsigned int body_colour[NUM_OF_BODIES];
} World;

static World world = {0};

enum colour
{
    COL_WHITE  = 0xFFFFFF,
    COL_RED    = 0xFF0000,
    COL_GREEN  = 0x00FF00,
    COL_BLUE   = 0x0000FF,
    COL_YELLOW = 0xFFFF00,
};

const unsigned int static_colour    = COL_RED;
const unsigned int normal_colour    = COL_WHITE;
const unsigned int player_colour    = COL_BLUE;
const unsigned int collision_colour = COL_YELLOW;

void World_init(void)
{
    const unsigned int padding  = 50;
    const unsigned int screen_w = Window_Width();
    const unsigned int screen_h = Window_Height();

    for (size_t i = 0; i < NUM_OF_BODIES; i++)
        world.bodies[i] = NULL;

    Rigid *ground = malloc(sizeof(*ground));
    *ground       = Rigid_Box_Init((vec2){(float)(screen_w / 2), (float)(screen_h)*0.70f},
                                   (float)(screen_w - padding * 4),
                                   40.0f,
                                   1.0f,
                                   1.0f,
                                   true);

    world.bodies[0]      = ground;
    world.body_colour[0] = COL_WHITE;
}

static void add_body_to_world(unsigned int x, unsigned int y)
{
    for (Rigid **p = &world.bodies[0], **end = &world.bodies[NUM_OF_BODIES]; p != end; p++)
    {
        if (*p == NULL)
        {
            Rigid *circle = malloc(sizeof(*circle));
            *circle       = Rigid_Circle_Init((vec2){(float)x, (float)y},
                                              20.0f,
                                              2.0f,
                                              0.5f,
                                              false);
            *p            = circle;
            break;
        }
    }
}

void World_update(const double elapsed_time_ms)
{
    const double elapsed_time_second = elapsed_time_ms / 1000.0;

    if (input_mouse_is_clicked(BUTTON_LEFT))
    {
        unsigned int mouse_x;
        unsigned int mouse_y;
        input_mouse_position(&mouse_x, &mouse_y);
        printf("Mouse (%d, %d)\n", mouse_x, mouse_y);

        add_body_to_world(mouse_x, mouse_y);
    }

    const float angle = 0.0f;
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        Rigid *body = world.bodies[i];

        if (body == NULL)
            continue;

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
    // for (size_t i = 0; i < NUM_OF_BODIES - 1; i++)
    //{
    //    Rigid *body1 = &world.bodies[i];

    //    for (size_t j = i + 1; j < NUM_OF_BODIES; j++)
    //    {
    //        Rigid *body2 = &world.bodies[j];

    //        if (body1->is_static && body2->is_static)
    //            continue;

    //        vec2  normal = {0};
    //        float depth  = 0.0f;

    //        if (collision_test(*body1, *body2, &normal, &depth))
    //        {
    //            world.body_colour[i] = collision_colour;
    //            world.body_colour[j] = collision_colour;

    //            if (body1->is_static)
    //            {
    //                const vec2 move_amount = vec2_mul_scal(normal, depth);
    //                Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
    //            }
    //            else if (body2->is_static)
    //            {
    //                const vec2 move_amount = vec2_mul_scal(normal, depth);
    //                Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
    //            }
    //            else
    //            {
    //                const vec2 move_amount = vec2_mul_scal(normal, depth / 2.0f);
    //                Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
    //                Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
    //            }

    //            collision_resolve(body1, body2, normal, depth);
    //        }
    //    }
    //}

    // SCREEN WRAPPING
    for (size_t i = 0; i < NUM_OF_BODIES && world.bodies[i]; i++)
    {
        Rigid *body = world.bodies[i];

        if (body == NULL)
            continue;

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
}

void World_on_render(void)
{
    for (size_t i = 0; i < NUM_OF_BODIES; i++)
    {
        const Rigid       *body   = world.bodies[i];
        const unsigned int colour = world.body_colour[i];

        if (body == NULL)
            continue;

        if (body->type == SHAPE_CIRCLE)
        {
            Window_Draw_Circle((int)body->radius, (int)body->position.x, (int)body->position.y, colour);
            // Window_Draw_Circle_Outline(body.radius, body.position.x, body.position.y, colour);
        }
        else if (body->type == SHAPE_BOX)
        {
            // Window_Draw_Rectangle((int)body.position.x, (int)body.position.y, (int)body.width, (int)body.height, colour);
            const unsigned int *ind   = body->indicies;
            vec2               *verts = body->transformed_verticies;

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
        Rigid_Destroy(world.bodies[i]);
    }
}
