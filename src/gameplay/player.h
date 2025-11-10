#pragma once

#include <stdbool.h>
#include <raylib.h>
#include "io/input.h"


typedef struct Player{
    Vector2 position; // coordenada lógica do player
    Vector2 velocity;
    Rectangle hitbox; // retângulo utilizado em colisões //
    bool isAlive;
    bool isOnGround;
}Player;


void player_init(Player* player); // inicia o player de forma padrão
void player_reset(Player* player, Vector2 spawnPosition); // reposiciona no spawn e limpa variáveis
void player_apply_input(Player* player, const InputState* input, float dt); // traduz o input ao player 
void player_update_hitbox(Player* player); // recalcula hitbox após inputs
Rectangle player_get_bounds(const Player* player); // é o retângulo de colisão do player no momento
