#pragma once
#include "core/state.h"

void gameover_screen_init(void);
void gameover_screen_update(float dt);
void gameover_screen_draw(void);
void gameover_screen_unload(void);

GameState gameover_screen_state(void);