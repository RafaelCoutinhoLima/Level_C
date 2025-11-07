#pragma once
#include "core/state.h"

void home_screen_init(void);

void home_screen_update(float dt);

void home_screen_draw(void);

void home_screen_unload(void);

GameState home_screen_state(void);