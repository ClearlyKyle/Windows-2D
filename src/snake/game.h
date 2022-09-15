#ifndef __GAME_H__
#define __GAME_H__

#include "../engine/app.h"

void Game_init(void);
void Game_update(const double elapsed_time);
void Game_on_render(void);
void Game_on_exit(void);

#endif // __GAME_H__