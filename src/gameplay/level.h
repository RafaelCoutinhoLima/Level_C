#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <raylib.h>
#include "gameplay/trap.h"  // TrapSet

// Dimensão máxima da grade (em tiles)
#define LEVEL_MAX_WIDTH   64
#define LEVEL_MAX_HEIGHT  32

// Tipos de tile para colisão
typedef enum TileType {
    TILE_EMPTY     = 0,
    TILE_SOLID     = 1,
    TILE_SEMISOLID = 2
} TileType;

// Representação do nível (grade estática 2D)
typedef struct Level {
    int id;

    int width;   // largura em tiles (<= LEVEL_MAX_WIDTH)
    int height;  // altura  em tiles (<= LEVEL_MAX_HEIGHT)

    // Grade de colisão: acessar como tiles[y][x]
    int tiles[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];

    // Índice do sprite no atlas por tile (y,x) — usado para desenhar o mapa
    int sprites[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];

    // Tamanho de um tile em pixels (ex.: 32.0f)
    float   tileSize;

    // Spawn do jogador em PIXELS (centro da célula/base do pé)
    Vector2 spawn;

    // Área de goal em PIXELS (x,y,width,height)
    Rectangle goal;

    // Conjunto de traps do nível
    TrapSet trapSet;
} Level;

/* Ciclo de vida */
void      level_init(Level* level);
void      level_clear(Level* level);

/* Consultas de colisão e bounds */
bool      level_is_tile_solid(const Level* level, int tx, int ty);
Rectangle level_tile_bounds(const Level* level, int tx, int ty);

/* Player helpers */
struct Player;
void      level_reset_player(const Level* level, struct Player* player);

/* Desenho dos tiles sólidos (usa Assets/DrawSprite) */
void      draw_level_tiles(const Level* level);

/* Carregamento por ID (via level_loader_from_id) */
bool      level_load_by_id(Level* level, int levelId);

#endif // LEVEL_H
