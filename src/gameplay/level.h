#pragma once

#include <raylib.h>
#include <stdbool.h>
#include "gameplay/trap.h"

#define LEVEL_MAX_WIDTH 64
#define LEVEL_MAX_HEIGHT 32

typedef enum TileType{
    TILE_EMPTY = 0,
    TILE_SOLID = 1,
    TILE_SEMISOLID = 2
}TileType;

typedef struct Level{
    int id;
    int width;
    int height;
    int tiles[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];

    float tileSize;
    Vector2 spawn;
    Rectangle goal;

    TrapSet trapSet;
}Level;

void level_init(Level* level);
void level_clear(Level* level);
bool level_is_tile_solid(const Level* level, int tx, int ty);
Rectangle level_tile_bounds(const Level* level, int tx, int ty);
struct Player;
void level_reset_player(const Level* level, struct Player* player);
