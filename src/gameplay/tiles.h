#ifndef TILES_H
#define TILES_H

#include <raylib.h>

#define TILE_SIZE      32
#define TILESET_COLS    8   // 256/32

typedef enum TileSprite {
    TSPR_FLOOR = 0,   // (0,0)
    TSPR_WALL  = 1,   // (0,1)
    TSPR_GRAVE = 2,   // (0,2)
    TSPR_PATH  = 3,   // (0,3)
    TSPR_SPAWN = 4,   // (0,4) (decorativo)
    TSPR_GOAL  = 5,   // (0,5)
    TSPR_TRAP  = 6    // (0,6)
} TileSprite;

static inline Rectangle tileset_src_from_index(int idx) {
    int col = idx % TILESET_COLS;
    int row = idx / TILESET_COLS;
    return (Rectangle){ col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };
}

#endif
