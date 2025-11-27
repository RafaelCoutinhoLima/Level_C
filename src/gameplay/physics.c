#include "physics.h"
#include "level.h"     
#include "io/input.h" 
#include "io/audio.h" 
#include <raylib.h>
#include <raymath.h>
#include <math.h>

// Valores Padrão 
static PhysicsParams gPhysBase = {
    .gravity = 1800.0f,
    .jumpSpeed = 520.0f,
    .maxFallSpeed = 750.0f,
    .jumpCutMultiplier = 1.8f,
    .fallGravityMultiplier = 1.35f
};

static PhysicsParams gPhysCurrent;

// Constantes de movimento
#define MOVE_ACCEL_GROUND  2000.0f
#define MOVE_ACCEL_AIR      800.0f
#define FRICTION_GROUND    1800.0f
#define FRICTION_AIR        200.0f
#define MOVE_SPEED_MAX      220.0f

void physics_init(const PhysicsParams* params){
    if (params) {
        gPhysBase = *params;
    }
    
    gPhysCurrent = gPhysBase;

    // Segurança para evitar gravidade zero
    if (gPhysCurrent.gravity <= 1.0f) gPhysCurrent.gravity = 1800.0f;
    if (gPhysCurrent.jumpSpeed <= 1.0f) gPhysCurrent.jumpSpeed = 520.0f;

    TraceLog(LOG_INFO, "[Physics] init -> g=%.1f jump=%.1f", gPhysCurrent.gravity, gPhysCurrent.jumpSpeed);
}

static void try_start_jump(Player* player, const InputState* input){
    if (!player || !input) return;
    
    if (input->jumpPressed && player->isOnGround) {
        player->velocity.y = -gPhysCurrent.jumpSpeed;
        player->isOnGround = false;
        
        audio_play_event(AUDIO_SFX_JUMP);

        TraceLog(LOG_DEBUG, "[Physics] jump -> velY=%.1f", player->velocity.y);
    }
}

static void apply_gravity(Player* player, const InputState* input, float dt){
    if (!player) return;

    float gravity = gPhysCurrent.gravity;
    
    if (player->velocity.y < 0.0f){
        if (!input || !input->jumpHeld){
            gravity *= gPhysCurrent.jumpCutMultiplier;
        }
    } else {
        gravity *= gPhysCurrent.fallGravityMultiplier;
    }
    
    player->velocity.y += gravity * dt;

    if (player->velocity.y > gPhysCurrent.maxFallSpeed){
        player->velocity.y = gPhysCurrent.maxFallSpeed;
    }
}

void physics_update(Player* player, const InputState* input, float dt){
    if (!player) return;
    if (dt <= 0.0f) dt = 1.0f / 60.0f;

    try_start_jump(player, input);
    apply_gravity(player, input, dt);
    float targetSpeed = 0.0f;
    
    if (input) {
        // moveAxis < 0 é esquerda, > 0 é direita
        if (input->moveAxis < -0.1f) {
            targetSpeed = -MOVE_SPEED_MAX;
        } 
        else if (input->moveAxis > 0.1f) {
            targetSpeed = MOVE_SPEED_MAX;
        }
    }

    float accel = player->isOnGround ? MOVE_ACCEL_GROUND : MOVE_ACCEL_AIR;
    float friction = player->isOnGround ? FRICTION_GROUND : FRICTION_AIR;

    if (targetSpeed != 0.0f) {
        // Acelerando
        if (player->velocity.x < targetSpeed) {
            player->velocity.x += accel * dt;
            if (player->velocity.x > targetSpeed) player->velocity.x = targetSpeed;
        } else {
            player->velocity.x -= accel * dt;
            if (player->velocity.x < targetSpeed) player->velocity.x = targetSpeed;
        }
    } else {
        // Parando (Atrito)
        if (player->velocity.x > 0) {
            player->velocity.x -= friction * dt;
            if (player->velocity.x < 0) player->velocity.x = 0;
        } else if (player->velocity.x < 0) {
            player->velocity.x += friction * dt;
            if (player->velocity.x > 0) player->velocity.x = 0;
        }
    }

    player->position.x += player->velocity.x * dt;
    player->position.y += player->velocity.y * dt;

    player_update_hitbox(player);
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

    TraceLog(LOG_INFO, "[Physics] configurada (scale=%.2f) -> g=%.1f", scale, gPhysCurrent.gravity);
}

void physics_apply_level_bounds(Player *player, const Level *level) {
    if (!player || !level) return;

    const float levelWidth  = level->width  * level->tileSize;

    if (player->position.x < player->hitbox.width/2) {
        player->position.x = player->hitbox.width/2;
        player->velocity.x = 0;
    } 
    else if (player->position.x > levelWidth - player->hitbox.width/2) {
        player->position.x = levelWidth - player->hitbox.width/2;
        player->velocity.x = 0;
    }
    
    if (player->position.y < -500) { 
        player->velocity.y = 100.0f; 
    }

    player_update_hitbox(player);
}