#include "player.h"
#include <raymath.h>
#include "render/draw_utils.h"
#include "io/assets.h"

static const Vector2 PLAYER_SIZE = {24.0f, 32.0f};
static const float PLAYER_MOVE_SPEED = 180.0f;

// Frame 0: Estático
// Frame 1-6: Andando
// Frame 7-9: Pulo
static const int PLAYER_START_FRAME[] = {0, 1, 7}; // Primeiro de cada estado
static const int PLAYER_FRAMES_PER_STATE[] = {1, 6, 3}; // Quantidade de frames de cada estado
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

    // Para pulo, 0(7) = subindo, 1(8) = ápice, 2(9) = caindo
    if (player->animState == PLAYER_ANIM_JUMP){
        if (player->velocity.y< -60.0f){
            player->frameIndex = 0;
        } else if (player->velocity.y < 60.0f){
            player->frameIndex = 1;
        } else{
            player->frameIndex = 2;
        }
        return;
    }

    // Para estático e correndo
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
    player->facingRight = true;
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
    player->facingRight = true;
    
    player_update_hitbox(player);

    TraceLog(LOG_INFO, "[Player] respawn em (%.2f, %.2f) velocidade limpa", player->position.x, player->position.y);
}

void player_apply_input(Player* player, const InputState* input, float dt){
    if (!player || !input)
        return;
    
    float previousVX = player->velocity.x;
    player->velocity.x = input->moveAxis * PLAYER_MOVE_SPEED; // calcula nova velocidade

    if (input->moveAxis > 0.0f){
        player->facingRight = true;
    } else if (input->moveAxis < 0.0f){
        player->facingRight = false;
    }

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

int player_get_absolute_frame(const Player* player){
    if (!player) return 0;

    int startFrame = PLAYER_START_FRAME[player->animState];
    return startFrame + player->frameIndex;
}