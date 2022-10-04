#ifndef __WORLD_H__
#define __WORLD_H__

#include "engine.h"

void World_init(void);
void World_update(const double elapsed_time_ms);
void World_on_render(void);
void World_on_exit(void);

#endif // __WORLD_H__