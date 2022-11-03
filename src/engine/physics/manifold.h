#ifndef __MANIFOLD_H__
#define __MANIFOLD_H__

#include "Rigid.h"

typedef struct Manifold
{
    Rigid *A;
    Rigid *B;
    float  penetration;
    vec2   normal;
} Manifold;

bool Manifold_AABB_Circle(Manifold *m);
bool Manifold_AABB_AABB(Manifold *m);
bool Manifold_Circle_Circle(Manifold *m);

#endif // __MANIFOLD_H__