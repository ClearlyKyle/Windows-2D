#include "manifold.h"

bool Manifold_AABB_Circle(Manifold *m)
{
    // Setup a couple pointers to each object
    Rigid *A = m->A;
    Rigid *B = m->B;

    // Vector from A to B
    const vec2 n = vec2_sub(B->position, A->position);

    // Closest point on A to center of B
    vec2 closest = n;

    // Calculate half extents along each axis
    const float x_extent = (A->aabb.max.x - A->aabb.min.x) / 2;
    const float y_extent = (A->aabb.max.y - A->aabb.min.y) / 2;

    // Clamp point to edges of the AABB
    closest.x = clamp_f(-x_extent, x_extent, closest.x);
    closest.y = clamp_f(-y_extent, y_extent, closest.y);

    bool inside = false;

    // Circle is inside the AABB, so we need to clamp the circle's center
    // to the closest edge
    if (vec2_cmp(n, closest))
    {
        inside = true;

        // Find closest axis
        if (fabsf(n.x) > fabsf(n.y))
        {
            // Clamp to closest extent
            if (closest.x > 0)
                closest.x = x_extent;
            else
                closest.x = -x_extent;
        }

        // y axis is shorter
        else
        {
            // Clamp to closest extent
            if (closest.y > 0)
                closest.y = y_extent;
            else
                closest.y = -y_extent;
        }
    }

    vec2  normal = vec2_sub(n, closest);
    float d      = vec2_length(normal);
    float r      = B->radius;

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (d > r * r && !inside)
        return false;

    // Avoided sqrt until we needed
    d = sqrtf(d);

    // Collision normal needs to be flipped to point outside if circle was
    // inside the AABB
    if (inside)
    {
        m->normal      = VEC2_NEGATIVE(n);
        m->penetration = r - d;
    }
    else
    {
        m->normal      = n;
        m->penetration = r - d;
    }

    return true;
}

bool Manifold_AABB_AABB(Manifold *m)
{
    // Setup a couple pointers to each object
    Rigid *A = m->A;
    Rigid *B = m->B;

    // Vector from A to B
    const vec2 n = vec2_sub(B->position, A->position);

    const AABB abox = A->aabb;
    const AABB bbox = B->aabb;

    // Calculate half extents along x axis for each object
    const float a_extent = (abox.max.x - abox.min.x) / 2;
    const float b_extent = (bbox.max.x - bbox.min.x) / 2;

    // Calculate overlap on x axis
    const float x_overlap = a_extent + b_extent - fabsf(n.x);

    // SAT test on x axis
    if (x_overlap > 0)
    {
        // Calculate half extents along x axis for each object
        const float a_extent = (abox.max.y - abox.min.y) / 2;
        const float b_extent = (bbox.max.y - bbox.min.y) / 2;

        // Calculate overlap on y axis
        const float y_overlap = a_extent + b_extent - fabsf(n.y);

        // SAT test on y axis
        if (y_overlap > 0)
        {
            // Find out which axis is axis of least penetration
            if (x_overlap > y_overlap)
            {
                // Point towards B knowing that n points from A to B
                if (n.x < 0.0f)
                    m->normal = (vec2){-1.0f, 0.0f};
                else
                    m->normal = (vec2){0.0f, 0.0f};

                m->penetration = x_overlap;
                return true;
            }
            else
            {
                // Point toward B knowing that n points from A to B
                if (n.y < 0)
                    m->normal = (vec2){0.0f, -1.0f};
                else
                    m->normal = (vec2){0.0f, 1.0f};
                m->penetration = y_overlap;
                return true;
            }
        }
    }
    return false;
}

bool Manifold_Circle_Circle(Manifold *m)
{
    // Setup a couple pointers to each object
    Rigid *A = m->A;
    Rigid *B = m->B;

    // Vector from A to B
    const vec2 n = vec2_sub(B->position, A->position);

    float r = A->radius + B->radius;
    r *= r;

    const float n_length = vec2_length(n);
    if (n_length > r)
        return false;

    // Circles have collided, now compute manifold
    float d = n_length; // perform actual sqrt

    // If distance between circles is not zero
    if (d != 0)
    {
        // Distance is difference between radius and distance
        m->penetration = r - d;

        // Utilize our d since we performed sqrt on it already within Length( )
        // Points from A to B, and is a unit vector
        m->normal = vec2_div_scal(n, d);
        return true;
    }

    // Circles are on same position
    else
    {
        // Choose random (but consistent) values
        m->penetration = A->radius;
        m->normal      = (vec2){1.0f, 0.0f};
        return true;
    }
}