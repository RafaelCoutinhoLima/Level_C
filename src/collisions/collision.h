#pragma once

#include <stdbool.h>
#include <raylib.h>

#include "gameplay/level.h"
#include "gameplay/trap.h"
#include "gameplay/player.h"

typedef struct CollisionResult{
    bool hitGround;
    bool hitCeiling;
    bool hitWallLeft;
    bool hitWallRight;
    bool hitTrap;
    bool reachedGoal;
    bool died;
}CollisionResult;

void collision_result_reset(CollisionResult* result);
void collisions_resolve_player_map(Player* player, const Level* level, CollisionResult* result);
void collisions_check_player_traps(Player* player, const TrapSet* trapSet, CollisionResult* result);
void collisions_check_goal(Player* player, const Rectangle* goalRect, CollisionResult* result);
