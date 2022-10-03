#ifndef __COLLISION_TESTING_H__
#define __COLLISION_TESTING_H__

#include "engine.h"

void Collision_testing_init(void);
void Collision_testing_update(const double elapsed_time_ms);
void Collision_testing_on_render(void);
void Collision_testing_on_exit(void);

#endif // __COLLISION_TESTING_H__