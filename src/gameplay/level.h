#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <raylib.h>
#include "gameplay/trap.h"  

// Dimensão máxima da grade 
#define LEVEL_MAX_WIDTH   64
#define LEVEL_MAX_HEIGHT  32

// Tipos de tile para colisão
typedef enum TileType {
    TILE_EMPTY     = 0,
    TILE_SOLID     = 1,
    TILE_SEMISOLID = 2
} TileType;

// Representação do nível 
typedef struct Level {
    int id;

    int width;   // largura em tiles 
    int height;  // altura  em tiles 

    // Grade de colisão x e y 
    int tiles[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];

    // Índice do sprite 
    int sprites[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];

    float   tileSize;
    // Spawn do jogador em PIXELS 
    Vector2 spawn;
    // Área de goal em PIXELS 
    Rectangle goal;
    TrapSet trapSet;
} Level;

void      level_init(Level* level);
void      level_clear(Level* level);

bool      level_is_tile_solid(const Level* level, int tx, int ty);
Rectangle level_tile_bounds(const Level* level, int tx, int ty);

struct Player;
void      level_reset_player(const Level* level, struct Player* player);

void      draw_level_tiles(const Level* level);

bool      level_load_by_id(Level* level, int levelId);

#endif 
