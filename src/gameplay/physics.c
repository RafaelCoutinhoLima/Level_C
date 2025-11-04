#include "physics.h"

#include <raylib.h>
#include <raymath.h>

static PhysicsParams gPhysDefaults = {
    .gravity = 1800.0f,
    .jumpSpeed = 520.0f,
    .maxFallSpeed = 750.0f,
    .jumpCutMultiplier = 1.8f,
    .fallGravityMultiplier = 1.35f
};

void physics_init(const PhysicsParams* params){
    if (params)
        gPhysDefaults = *params;

    TraceLog(LOG_INFO, "[Physics] init -> g=%.1f jump=%.1f maxFall=%.1f cut=%.2f fallMul=%.2f", gPhysDefaults.gravity, gPhysDefaults.jumpSpeed, gPhysDefaults.maxFallSpeed, gPhysDefaults.jumpCutMultiplier, gPhysDefaults.fallGravityMultiplier);
}

static void try_start_jump(Player* player, const InputState* input){
    if (!player || !input)
        return;
    
    if (!input->jumpPressed)
        return;
    
    if (!player->isOnGround)
        return;

    player->velocity.y = -gPhysDefaults.jumpSpeed;
    player->isOnGround = false;

    TraceLog(LOG_DEBUG, "[Physics] jump -> velY=%.1f", player->velocity.y);
}

static void apply_gravity(Player* player, const InputState* input, float dt){
    if (!player)
        return;

    float gravity = gPhysDefaults.gravity;
    if (player->velocity.y < 0.0f){
        if (!input || !input->jumpHeld){
            gravity *= gPhysDefaults.jumpCutMultiplier;
        }
    }else{
        gravity *= gPhysDefaults.fallGravityMultiplier;
    }
    player->velocity.y += gravity * dt;

    if (player->velocity.y > gPhysDefaults.maxFallSpeed){
        player->velocity.y = gPhysDefaults.maxFallSpeed;
    }
}

void physics_update(Player* player, const InputState* input, float dt){
    if (!player)
        return;

    if (dt <= 0.0f){
        dt = 1.0f / 60.0f;
    }
    TraceLog(LOG_TRACE, "[Physics] pre pos(%.2f, %.2f) vel(%.2f, %.2f)", player->position.x, player->position.y, player->velocity.x, player->velocity.y);

    try_start_jump(player, input);
    apply_gravity(player, input, dt);

    player->position.x += player->velocity.x * dt;
    player->position.y += player->velocity.y * dt;

    TraceLog(LOG_TRACE, "[Physics] post pos (%.2f, %.2f) vel(%.2f, %.2f)", player->position.x, player->position.y, player->velocity.x, player->velocity.y);
}
