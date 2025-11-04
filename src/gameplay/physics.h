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

void physics_init(const PhysicsParams* params);
void physics_update(Player* player, const InputState* input, float dt);
