#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include <raylib.h>
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "gameplay/trap.h"
#include "io/input.h"


void DrawSprite(Rectangle source_rect,Vector2 position);
//desenhar o sprite dentro do mapa o atlas

void DrawSpriteAdvanced(Rectangle source_rect,Rectangle dest_rect,Color tint);
//movendo as funções para ca 
void draw_level_tiles(const Level* level);
void draw_traps(const TrapSet* trapset);
void draw_player(const Player* player);
//modificar o hud para receber os dados para n usar variaver globais de outras telas
void draw_hud(const Player* player,const Level* level,const InputState* input);

#endif