#include "level.h"
#include "player.h"
#include <string.h>
#include <raylib.h>

static const float DEFAULT_TILE_SIZE = 32.0f;

void level_init(Level* level){
        if (!level)
            return;

        memset(level, 0, sizeof(*level));
        level->tileSize = DEFAULT_TILE_SIZE;
        trap_set_init(&level->trapSet);

        TraceLog(LOG_INFO, "[Level] init");
}

void level_clear(Level* level){
    if (!level)
        return;

    trap_set_clear(&level->trapSet);
    memset(level->tiles, 0, sizeof(level->tiles));
    level->width = 0;
    level->height = 0;

    TraceLog(LOG_INFO, "[Level] clear");
}

bool level_is_tile_solid(const Level* level, int tx, int ty){
    if (!level)
        return false;

    if (tx < 0 || ty < 0 || tx >= level->width || ty >= level->height){
        return true;
    }
    return level->tiles[ty][tx] == TILE_SOLID;
}

Rectangle level_tile_bounds(const Level* level, int tx, int ty){
    if (!level)
        return (Rectangle){0};

    float size = level->tileSize;
    return (Rectangle){
        .x = tx * size,
        .y = ty * size,
        .width = size,
        .height = size
    };
}

void level_reset_player(const Level* level, Player* player){
    if (!level || !player)
        return;

    player_reset(player, level->spawn);
}
