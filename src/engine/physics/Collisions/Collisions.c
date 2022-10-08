#include "Collisions.h"

static void _project_box_verticies(const vec2 *verticies, const size_t num_verts, const vec2 axis, float *min, float *max)
{
    *min = FLT_MAX;
    *max = FLT_MIN;

    for (size_t i = 0; i < num_verts; i++)
    {
        vec2        vert       = verticies[i];
        const float projection = vec2_dot(vert, axis);

        if (projection < *min)
            *min = projection;
        if (projection > *max)
            *max = projection;
    }
}

static void _project_circle(const vec2 center, const float radius, const vec2 axis, float *min, float *max)
{
    const vec2 direction            = vec2_normalise(axis);
    const vec2 direction_and_radius = vec2_mul_scal(direction, radius);

    const vec2 p1 = vec2_add(center, direction_and_radius);
    const vec2 p2 = vec2_sub(center, direction_and_radius);

    *min = vec2_dot(p1, axis);
    *max = vec2_dot(p2, axis);

    // Swap if they are wrong way round?
    if (*min > *max)
    {
        const float t = *min;
        *min          = *max;
        *max          = t;
    }
}

static int _find_closest_point(const vec2 circle_center, const vec2 *verticies, const size_t num_verts)
{
    int   result       = -1;
    float min_distance = FLT_MAX;

    for (size_t i = 0; i < num_verts; i++)
    {
        const vec2  v        = verticies[i];
        const float distance = vec2_distance(v, circle_center);

        if (distance < min_distance)
        {
            min_distance = distance;
            result       = (int)i;
        }
    }

    return result;
}

bool collision_circle_circle(const Rigid circle1, const Rigid circle2, vec2 *normal, float *depth)
{
    assert(circle1.type == SHAPE_CIRCLE && "Circle_intersection - circle1 is not a CIRCLE\n");
    assert(circle2.type == SHAPE_CIRCLE && "Circle_intersection - circle2 is not a CIRCLE\n");

    const float distance        = vec2_distance(circle1.position, circle2.position);
    const float combined_radius = circle1.radius + circle2.radius;

    if (distance >= combined_radius)
        return false;

    *normal = vec2_sub(circle2.position, circle1.position);
    *normal = vec2_normalise(*normal);

    *depth = combined_radius - distance;

    return true;
}

bool collision_box_box(const Rigid box1, const Rigid box2, vec2 *normal, float *depth)
{
    assert(box1.type == SHAPE_BOX && "Box_intersection - box1 is not a BOX\n");
    assert(box2.type == SHAPE_BOX && "Box_intersection - box2 is not a BOX\n");

    float min1, max1;
    float min2, max2;

    vec2  local_normal = (vec2){0.0f, 0.0f};
    float local_depth  = FLT_MAX;

    for (size_t i = 0; i < box1.vert_count; i++)
    {
        const vec2 vert1 = box1.transformed_verticies[i];
        const vec2 vert2 = box1.transformed_verticies[(i + 1) % box1.vert_count];

        const vec2 edge = vec2_sub(vert2, vert1);
        vec2       axis = (vec2){-edge.y, edge.x};
        axis            = vec2_normalise(axis);

        _project_box_verticies(box1.transformed_verticies, box1.vert_count, axis, &min1, &max1);
        _project_box_verticies(box2.transformed_verticies, box2.vert_count, axis, &min2, &max2);

        if (min1 >= max2 || min2 >= max1)
        {
            return false; // seperated
        }

        const float axis_depth = min((max2 - min1), (max1 - min2));

        if (axis_depth < local_depth)
        {
            local_depth  = axis_depth;
            local_normal = axis;
        }
    }

    for (size_t i = 0; i < box2.vert_count; i++)
    {
        const vec2 vert1 = box2.transformed_verticies[i];
        const vec2 vert2 = box2.transformed_verticies[(i + 1) % box2.vert_count];

        const vec2 edge = vec2_sub(vert2, vert1);
        vec2       axis = (vec2){-edge.y, edge.x};
        axis            = vec2_normalise(axis);

        _project_box_verticies(box1.transformed_verticies, box1.vert_count, axis, &min1, &max1);
        _project_box_verticies(box2.transformed_verticies, box2.vert_count, axis, &min2, &max2);

        if (min1 >= max2 || min2 >= max1)
        {
            return false; // seperated
        }

        const float axis_depth = min((max2 - min1), (max1 - min2));

        if (axis_depth < local_depth)
        {
            local_depth  = axis_depth;
            local_normal = axis;
        }
    }

    const vec2 direction = vec2_sub(box2.position, box1.position);

    if (vec2_dot(direction, local_normal) < 0.0f)
        *normal = (vec2){-local_normal.x, -local_normal.y};
    else
        *normal = local_normal;

    *depth = local_depth;

    return true; // touching
}

bool collision_box_circle(const Rigid box, const Rigid circle, vec2 *normal, float *depth)
{
    assert(box.type == SHAPE_BOX && "Box_Circle_collision - box is not a BOX\n");
    assert(circle.type == SHAPE_CIRCLE && "Box_Circle_collision - circle is not a BOX\n");

    float min1, max1;
    float min2, max2;

    const vec2  circle_center = circle.position;
    const float circle_radius = circle.radius;

    vec2  local_normal = (vec2){0.0f, 0.0f};
    float local_depth  = FLT_MAX;

    vec2  axis       = {0};
    float axis_depth = 0.0f;

    for (size_t i = 0; i < box.vert_count; i++)
    {
        const vec2 vert1 = box.transformed_verticies[i];
        const vec2 vert2 = box.transformed_verticies[(i + 1) % box.vert_count];

        const vec2 edge = vec2_sub(vert2, vert1);
        axis            = (vec2){-edge.y, edge.x};
        axis            = vec2_normalise(axis);

        _project_box_verticies(box.transformed_verticies, box.vert_count, axis, &min1, &max1);
        _project_circle(circle_center, circle_radius, axis, &min2, &max2);

        if (min1 >= max2 || min2 >= max1)
        {
            return false; // seperated
        }

        axis_depth = min((max2 - min1), (max1 - min2));

        if (axis_depth < local_depth)
        {
            local_depth  = axis_depth;
            local_normal = axis;
        }
    }

    const int  closest_point_index = _find_closest_point(circle_center, box.verticies, box.vert_count);
    const vec2 closest_point       = box.verticies[closest_point_index];

    axis = vec2_sub(closest_point, circle_center);
    axis = vec2_normalise(axis);

    _project_box_verticies(box.transformed_verticies, box.vert_count, axis, &min1, &max1);
    _project_circle(circle_center, circle_radius, axis, &min2, &max2);

    if (min1 >= max2 || min2 >= max1)
    {
        return false; // seperated
    }

    axis_depth = min((max2 - min1), (max1 - min2));

    if (axis_depth < local_depth)
    {
        local_depth  = axis_depth;
        local_normal = axis;
    }

    const vec2 direction = vec2_sub(box.position, circle_center);

    if (vec2_dot(direction, local_normal) < 0.0f)
        *normal = (vec2){-local_normal.x, -local_normal.y};
    else
        *normal = local_normal;

    *depth = local_depth;

    return true;
}

bool collision_test(const Rigid body1, const Rigid body2, vec2 *normal, float *depth)
{
    const enum Shape_Type body1_type = body1.type;
    const enum Shape_Type body2_type = body2.type;

    *normal = (vec2){0.0f, 0.0f};
    *depth  = 0.0f;

    if (body1_type == SHAPE_BOX)
    {
        if (body2_type == SHAPE_BOX)
        {
            return collision_box_box(body1, body2, normal, depth);
        }
        else if (body2_type == SHAPE_CIRCLE)
        {
            const bool res = collision_box_circle(body1, body2, normal, depth);
            vec2_make_negative(normal);
            return res;
        }
    }
    if (body1_type == SHAPE_CIRCLE)
    {
        if (body2_type == SHAPE_BOX)
        {
            return collision_box_circle(body2, body1, normal, depth);
        }
        else if (body2_type == SHAPE_CIRCLE)
        {
            return collision_circle_circle(body1, body2, normal, depth);
        }
    }

    fprintf(stderr, "No collision test for types : %d, %d\n", body1_type, body2_type);
    return false;
}