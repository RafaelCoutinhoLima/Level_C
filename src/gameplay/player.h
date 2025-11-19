#pragma once

#include <stdbool.h>
#include <raylib.h>
#include "io/input.h"

#define PLAYER_FRAME_WIDTH 24
#define PLAYER_FRAME_HEIGHT 32

typedef enum {
    PLAYER_ANIM_IDLE = 0,
    PLAYER_ANIM_RUN = 1,
    PLAYER_ANIM_JUMP = 2,
} PlayerAnimState;

typedef struct Player{
    Vector2 position; // coordenada lógica do player
    Vector2 velocity;
    Rectangle hitbox; // retângulo utilizado em colisões //
    bool isAlive;
    bool isOnGround;
    bool facingRight;

    PlayerAnimState animState;
    int frameIndex;
    float frameTimer;
}Player;

void player_anim_update(Player* player, float dt);

void player_init(Player* player); // inicia o player de forma padrão
void player_reset(Player* player, Vector2 spawnPosition); // reposiciona no spawn e limpa variáveis
void player_apply_input(Player* player, const InputState* input, float dt); // traduz o input ao player 
void player_update_hitbox(Player* player); // recalcula hitbox após inputs
Rectangle player_get_bounds(const Player* player); // é o retângulo de colisão do player no momento

int player_get_absolute_frame(const Player* player);