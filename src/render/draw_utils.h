#pragma once
#include <raylib.h>
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "gameplay/trap.h"
#include "io/input.h"

void draw_level_tiles(const Level* level);
void draw_traps(const TrapSet* trapset);
void draw_player(const Player* player);
void draw_level_map(const Level* L);
void draw_goal(const Level* level);