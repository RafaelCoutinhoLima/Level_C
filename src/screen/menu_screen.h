#pragma once
#include "core/state.h"

void menu_screen_init(void);
void menu_screen_update(float dt);
void menu_screen_draw(void);
void menu_screen_unload(void);

GameState menu_screen_state(void);