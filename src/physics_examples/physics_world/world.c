#include "World.h"

#define MAX_NUM_OF_BODIES 10

typedef struct World
{
    size_t       current_number_of_bodies;
    Rigid       *bodies[MAX_NUM_OF_BODIES];
    unsigned int body_colour[MAX_NUM_OF_BODIES];
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

const static unsigned int static_colour    = COL_RED;
const static unsigned int normal_colour    = COL_WHITE;
const static unsigned int player_colour    = COL_BLUE;
const static unsigned int collision_colour = COL_YELLOW;

const static vec2 WORLD_GRAVITY = {0.0f, 9.81f};

void World_init(void)
{
    const unsigned int padding  = 50;
    const unsigned int screen_w = Window_Width();
    const unsigned int screen_h = Window_Height();

    for (size_t i = 0; i < MAX_NUM_OF_BODIES; i++)
        world.bodies[i] = NULL;

    Rigid *ground = malloc(sizeof(*ground));
    *ground       = Rigid_Box_Init((vec2){(float)(screen_w / 2), (float)(screen_h)*0.70f},
                                   (float)(screen_w - padding * 4),
                                   40.0f,
                                   1.0f,
                                   1.0f,
                                   true);

    world.bodies[0]                = ground;
    world.body_colour[0]           = COL_RED;
    world.current_number_of_bodies = 1;
}

static void add_body_to_world(unsigned int x, unsigned int y, enum Shape_Type type)
{
    if (world.current_number_of_bodies == MAX_NUM_OF_BODIES)
        return;

    Rigid **p = &world.bodies[world.current_number_of_bodies];

    if (*p == NULL)
    {
        Rigid *body = malloc(sizeof(*body));
        if (type == SHAPE_CIRCLE)
        {
            *body = Rigid_Circle_Init((vec2){(float)x, (float)y},
                                      20.0f,
                                      2.0f,
                                      0.5f,
                                      false);
        }
        else if (type == SHAPE_BOX)
        {
            *body = Rigid_Box_Init((vec2){(float)x, (float)y},
                                   50.0f,
                                   50.0f,
                                   2.0f,
                                   0.6f,
                                   false);
        }
        *p = body;
        world.current_number_of_bodies++;
    }
}

void World_update(const double elapsed_time_ms)
{
    const double elapsed_time_second = elapsed_time_ms / 1000.0;
    // printf("elapsed_time_second : %f\n", elapsed_time_second);

    if (input_mouse_is_clicked(BUTTON_LEFT))
    {
        unsigned int mouse_x;
        unsigned int mouse_y;
        input_mouse_position(&mouse_x, &mouse_y);
        printf("Mouse (%d, %d)\n", mouse_x, mouse_y);

        add_body_to_world(mouse_x, mouse_y, SHAPE_CIRCLE);
    }
    if (input_mouse_is_clicked(BUTTON_RIGHT))
    {
        unsigned int mouse_x;
        unsigned int mouse_y;
        input_mouse_position(&mouse_x, &mouse_y);
        printf("Mouse (%d, %d)\n", mouse_x, mouse_y);

        add_body_to_world(mouse_x, mouse_y, SHAPE_BOX);
    }

    const double iteration_time = elapsed_time_second / (double)NUMBER_OF_ITERATIONS;
    for (size_t iteration = 0; iteration < NUMBER_OF_ITERATIONS; iteration++)
    {
        const float angle = 0.0f;
        for (size_t i = 0; i < world.current_number_of_bodies; i++)
        {
            Rigid *body = world.bodies[i];

            Rigid_step(body, (const float)iteration_time, WORLD_GRAVITY);

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
        for (size_t i = 0; i < world.current_number_of_bodies; i++)
        {
            Rigid *body1 = world.bodies[i];

            for (size_t j = i + 1; j < world.current_number_of_bodies; j++)
            {
                Rigid *body2 = world.bodies[j];

                if (body1->is_static && body2->is_static)
                    continue;

                vec2  normal = {0};
                float depth  = 0.0f;

                if (collision_test(*body1, *body2, &normal, &depth))
                {
                    if (body1->is_static)
                    {
                        const vec2 move_amount = vec2_mul_scal(normal, depth);
                        Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
                    }
                    else if (body2->is_static)
                    {
                        const vec2 move_amount = vec2_mul_scal(normal, depth);
                        Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
                    }
                    else
                    {
                        const vec2 move_amount = vec2_mul_scal(normal, depth / 2.0f);
                        Rigid_Move_Amount_2D(&body1->position, -move_amount.x, -move_amount.y);
                        Rigid_Move_Amount_2D(&body2->position, move_amount.x, move_amount.y);
                    }

                    collision_resolve(body1, body2, normal, depth);
                }
            }
        }

        for (size_t i = 0; i < world.current_number_of_bodies; i++)
        {
            Rigid *const body = world.bodies[i];

            // check if body is bellow bottom of the screen
            AABB_Rigid_Update(body);
            const AABB aabb = body->aabb;
            if (aabb.max.y > (float)Window_Height())
            {
                world.bodies[i]                                  = world.bodies[world.current_number_of_bodies - 1];
                world.bodies[world.current_number_of_bodies - 1] = NULL;
                world.current_number_of_bodies--;
            }
        }

        for (size_t i = 0; i < world.current_number_of_bodies; i++)
        {
            Rigid *const body = world.bodies[i];

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
}

void World_on_render(void)
{
    for (size_t i = 0; i < world.current_number_of_bodies; i++)
    {
        const Rigid       *body   = world.bodies[i];
        const unsigned int colour = world.body_colour[i];

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
    for (size_t i = 0; i < world.current_number_of_bodies; i++)
    {
        Rigid_Destroy(world.bodies[i]);
    }
}
