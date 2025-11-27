#include "level.h"
#include "player.h"

#include <string.h>
#include <raylib.h>

#include "render/draw_utils.h"     
#include "io/assets.h"             
#include "levels/level_loader.h"   

static const float DEFAULT_TILE_SIZE = 32.0f;

//Helpers
static inline bool in_bounds(const Level* L, int tx, int ty) {
    return (L && tx >= 0 && ty >= 0 && tx < L->width && ty < L->height);
}

//Ciclo de vida
void level_init(Level* level) {
    if (!level) return;

    memset(level, 0, sizeof(*level));
    level->tileSize = DEFAULT_TILE_SIZE;
    trap_set_init(&level->trapSet);

    TraceLog(LOG_INFO, "[Level] init");
}

void level_clear(Level* level) {
    if (!level) return;

    trap_set_clear(&level->trapSet);

    // zera a grade e metadados
    memset(level->tiles, 0, sizeof(level->tiles));
    level->width  = 0;
    level->height = 0;
    level->id     = 0;
    level->spawn  = (Vector2){0,0};
    level->goal   = (Rectangle){0,0,0,0};

    TraceLog(LOG_INFO, "[Level] clear");
}

//Consultas
bool level_is_tile_solid(const Level* level, int tx, int ty) {
    if (!level) return false;

    // Fora do mapa = bloqueado
    if (!in_bounds(level, tx, ty)) return true;
    return level->tiles[ty][tx] == TILE_SOLID;
}

Rectangle level_tile_bounds(const Level* level, int tx, int ty) {
    if (!level) return (Rectangle){0};

    float s = level->tileSize;
    return (Rectangle){ tx * s, ty * s, s, s };
}

//Player
void level_reset_player(const Level* level, struct Player* player) {
    if (!level || !player) return;
    player_reset(player, level->spawn);
}
bool level_load_by_id(Level* level, int levelId) {
    if (!level) return false;

    level->id = levelId;

    trap_set_clear(&level->trapSet);
    memset(level->tiles, 0, sizeof(level->tiles));
    level->width  = 0;
    level->height = 0;
    level->spawn  = (Vector2){0,0};
    level->goal   = (Rectangle){0,0,0,0};

    if (!level_loader_from_id(levelId, level)) {
        TraceLog(LOG_ERROR, "[Level] falha ao carregar id=%d", levelId);
        return false;
    }

    TraceLog(LOG_INFO,
        "[Level] carregado id=%d (W=%d H=%d) spawn(%.0f,%.0f) goal(x=%.0f,y=%.0f,w=%.0f,h=%.0f)",
        levelId, level->width, level->height,
        level->spawn.x, level->spawn.y,
        level->goal.x, level->goal.y, level->goal.width, level->goal.height);

    return true;
}
