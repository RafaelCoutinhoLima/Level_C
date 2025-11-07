#pragma once
#include "core/state.h"

void play_screen_init(void);
void play_screen_update(float dt);
void play_screen_draw(void);
void play_screen_unload(void);

GameState play_screen_state(void);