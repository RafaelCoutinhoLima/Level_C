#include "physics.h"
#include "level.h"
#include <raylib.h>
#include <raymath.h>

static PhysicsParams gPhysBase = {
    .gravity = 1800.0f,
    .jumpSpeed = 520.0f,
    .maxFallSpeed = 750.0f,
    .jumpCutMultiplier = 1.8f,
    .fallGravityMultiplier = 1.35f
};

static PhysicsParams gPhysCurrent;

void physics_init(const PhysicsParams* params){
    if (params)
        gPhysBase = *params;

    TraceLog(LOG_INFO, "[Physics] init -> g=%.1f jump=%.1f maxFall=%.1f cut=%.2f fallMul=%.2f", gPhysCurrent.gravity, gPhysCurrent.jumpSpeed, gPhysBase.maxFallSpeed, gPhysCurrent.jumpCutMultiplier, gPhysCurrent.fallGravityMultiplier);
}

static void try_start_jump(Player* player, const InputState* input){
    if (!player || !input)
        return;
    
    if (!input->jumpPressed)
        return;
    
    if (!player->isOnGround)
        return;

    player->velocity.y = -gPhysCurrent.jumpSpeed;
    player->isOnGround = false;

    TraceLog(LOG_DEBUG, "[Physics] jump -> velY=%.1f", player->velocity.y);
}

static void apply_gravity(Player* player, const InputState* input, float dt){
    if (!player)
        return;

    float gravity = gPhysCurrent.gravity;
    if (player->velocity.y < 0.0f){
        if (!input || !input->jumpHeld){
            gravity *= gPhysCurrent.jumpCutMultiplier;
        }
    }else{
        gravity *= gPhysCurrent.fallGravityMultiplier;
    }
    player->velocity.y += gravity * dt;

    if (player->velocity.y > gPhysCurrent.maxFallSpeed){
        player->velocity.y = gPhysCurrent.maxFallSpeed;
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

void physics_configure_for_level(const Level *level){
    if (!level){
        gPhysCurrent = gPhysBase;
        return;
    }

    const float referenceTile = 32.0f;
    const float tileSize = (level->tileSize > 0.0f) ? level->tileSize : referenceTile;
    const float scale = tileSize / referenceTile;

    gPhysCurrent.gravity = gPhysBase.gravity * scale;
    gPhysCurrent.jumpSpeed = gPhysBase.jumpSpeed * scale;
    gPhysCurrent.maxFallSpeed = gPhysBase.maxFallSpeed * scale;
    gPhysCurrent.jumpCutMultiplier = gPhysBase.jumpCutMultiplier;
    gPhysCurrent.fallGravityMultiplier = gPhysBase.fallGravityMultiplier;

    TraceLog(LOG_INFO, "[Physics] configurada para nível %d (tile %.1f, escala %.2f)", level->id, tileSize, scale);
}

void physics_apply_level_bounds(Player *player, const Level *level) {
    if (!player || !level) return;

    const float levelWidth  = level->width  * level->tileSize;
    const float levelHeight = level->height * level->tileSize;

    if (levelWidth <= 0.0f || levelHeight <= 0.0f) return;
    if (player->hitbox.width <= 0.0f || player->hitbox.height <= 0.0f) return;

    const float minX = 0.0f;
    const float minY = 0.0f;
    const float maxX = levelWidth  - player->hitbox.width;
    const float maxY = levelHeight - player->hitbox.height;

    if (maxX < minX || maxY < minY) {
        player_update_hitbox(player);
        return;
    }

    if (player->hitbox.x < minX) {
        player->hitbox.x = minX;
        if (player->velocity.x < 0.0f) player->velocity.x = 0.0f;
    } else if (player->hitbox.x > maxX) {
        player->hitbox.x = maxX;
        if (player->velocity.x > 0.0f) player->velocity.x = 0.0f;
    }

    if (player->hitbox.y < minY) {
        player->hitbox.y = minY;
        if (player->velocity.y < 0.0f) player->velocity.y = 0.0f;
    } else if (player->hitbox.y > maxY) {
        player->hitbox.y = maxY;
        if (player->velocity.y > 0.0f) player->velocity.y = 0.0f;
        player->isOnGround = true;
    }

    player->position.x = player->hitbox.x + player->hitbox.width * 0.5f;
    player->position.y = player->hitbox.y + player->hitbox.height;

    player_update_hitbox(player);
}


