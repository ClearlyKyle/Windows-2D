#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "../Rigid.h"

#include <stdbool.h>
#include <assert.h>

bool collision_circle_circle(const Rigid circle1, const Rigid circle2, vec2 *normal, float *depth);
bool collision_box_box(const Rigid box1, const Rigid box2, vec2 *normal, float *depth);
bool collision_box_circle(const Rigid box, const Rigid circle, vec2 *normal, float *depth);

bool collision_test(const Rigid body1, const Rigid body2, vec2 *normal, float *depth);
void collision_resolve(Rigid *body1, Rigid *body2, const vec2 normal, const float depth);

#endif // __COLLISIONS_H__