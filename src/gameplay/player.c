#include "player.h"
#include <raymath.h>
#include "render/draw_utils.h"
#include "io/assets.h"

static const Vector2 PLAYER_SIZE = {24.0f, 32.0f};
static const float PLAYER_MOVE_SPEED = 180.0f;

static Rectangle make_hitbox(Vector2 center){
    return (Rectangle){
        .x = center.x - (PLAYER_SIZE.x * 0.5f),
        .y = center.y - PLAYER_SIZE.y,
        .width = PLAYER_SIZE.x,
        .height = PLAYER_SIZE.y
    };
}

void player_init(Player* player){
    if (!player)
        return;
    
    player->position = (Vector2){0}; 
    player->velocity = (Vector2){0};
    player->hitbox = make_hitbox(player->position); // hitbox em 0,0
    player->isAlive = true;
    player->isOnGround = true;

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