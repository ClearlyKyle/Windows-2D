#ifndef __PHYSICS_TEST_H__
#define __PHYSICS_TEST_H__

#include "../engine/app.h"
#include "../engine/input.h"

void Physics_init(void);
void Physics_update(const double elapsed_time_ms);
void Physics_on_render(void);
void Physics_on_exit(void);

#endif // __PHYSICS_TEST_H__