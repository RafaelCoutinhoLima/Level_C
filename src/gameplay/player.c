#include "player.h"
#include <raymath.h>
#include "render/draw_utils.h"
#include "io/assets.h"

static const Vector2 PLAYER_SIZE = {30.0f, 40.0f};
static const float PLAYER_MOVE_SPEED = 180.0f;

static const int PLAYER_FRAMES_PER_STATE[] = {4, 6, 4};
static const float PLAYER_FPS_PER_STATE[] = {6.0f, 10.0f, 8.0f};
static const float PLAYER_RUN_THRESHOLD = 20.0f;

static Rectangle make_hitbox(Vector2 center){
    return (Rectangle){
        .x = center.x - (PLAYER_SIZE.x * 0.5f),
        .y = center.y - PLAYER_SIZE.y,
        .width = PLAYER_SIZE.x,
        .height = PLAYER_SIZE.y
    };
}

static void player_set_anim_state(Player* player, PlayerAnimState newState){
    if (player->animState != newState){
        player->animState = newState;
        player->frameIndex = 0;
        player->frameTimer = 0.0f;
    }
}

static void player_update_animation(Player* player, float dt){
    if (!player) return;

    if (player->animState == PLAYER_ANIM_JUMP){
        player->frameTimer = 0.0f;

        if (player->velocity.y < -120.0f){
            player->frameIndex = 1;
        }else if (player->velocity.y < 0.0f){
            player->frameIndex = 2;
        }else if (player->velocity.y < 160.0f){
            player->frameIndex = 3;
        }else{
            player->frameIndex = 0;
        }
        return;
    }

    int frameCount = PLAYER_FRAMES_PER_STATE[player->animState];
    float fps = PLAYER_FPS_PER_STATE[player->animState];

    player->frameTimer += dt;
    if (player->frameTimer >= 1.0f / fps){
        player->frameTimer -= 1.0f / fps;
        player->frameIndex = (player->frameIndex + 1) % frameCount;
    }
}

static void player_update_anim_state(Player* player){
    if (!player) return;

    if (!player->isOnGround){
        player_set_anim_state(player, PLAYER_ANIM_JUMP);
        return;
    }
    float speed = fabsf(player->velocity.x);
    if (speed < PLAYER_RUN_THRESHOLD){
        player_set_anim_state(player, PLAYER_ANIM_IDLE);
    }else{
        player_set_anim_state(player, PLAYER_ANIM_RUN);
    }
}

void player_anim_update(Player* player, float dt){
    if (!player) return;
    player_update_anim_state(player);
    player_update_animation(player, dt);
}

void player_init(Player* player){
    if (!player)
        return;
    
    player->position = (Vector2){0}; 
    player->velocity = (Vector2){0};
    player->hitbox = make_hitbox(player->position); // hitbox em 0,0
    player->isAlive = true;
    player->isOnGround = true;
    player->animState = PLAYER_ANIM_IDLE;
    player->frameIndex = 0;
    player->frameTimer = 0.0f;

    TraceLog(LOG_INFO, "[Player] init -> pos(%.2f, %.2f)", player->position.x, player->position.y);
}

void player_reset(Player* player, Vector2 spawnPosition){
    if (!player)
        return;

    TraceLog(LOG_DEBUG, "[Player] reset iniciado");
    
    player->position = spawnPosition; 
    player->velocity = (Vector2){0};
    player->hitbox = make_hitbox(player->position); // recalcula hitbox
    player->isAlive = true;
    player->isOnGround = false;
    
    player_update_hitbox(player);

    TraceLog(LOG_INFO, "[Player] respawn em (%.2f, %.2f) velocidade limpa", player->position.x, player->position.y);
}

void player_apply_input(Player* player, const InputState* input, float dt){
    if (!player || !input)
        return;
    
    float previousVX = player->velocity.x;
    player->velocity.x = input->moveAxis * PLAYER_MOVE_SPEED; // calcula nova velocidade

    TraceLog(LOG_DEBUG, "[Player] input -> axis=%.2f velX: %.2f -> %.2f dt= %.3f", input->moveAxis, previousVX, player->velocity.x, dt); 
}

void player_update_hitbox(Player* player){
    if (!player)
        return;
    
    player->hitbox = make_hitbox(player->position); // recalcula hitbox
}

Rectangle player_get_bounds(const Player* player){
    if (!player){
        Rectangle empty = {0};
        return empty;
    }
    return player->hitbox;
}