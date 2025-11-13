#pragma once

#include <raylib.h>
#include "gameplay/player.h"

typedef struct PhysicsParams{
    float gravity;
    float jumpSpeed;
    float maxFallSpeed;
    float jumpCutMultiplier;
    float fallGravityMultiplier;
}PhysicsParams;

typedef struct Level Level;

void physics_init(const PhysicsParams* params);
void physics_update(Player* player, const InputState* input, float dt);
void physics_configure_for_level(const Level *level);
void physics_apply_level_bounds(Player *player, const Level *level);
