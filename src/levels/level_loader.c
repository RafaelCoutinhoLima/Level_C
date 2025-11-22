// src/levels/level_loader.c
#include "level_loader.h"
#include "gameplay/level.h"
#include "gameplay/trap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>           // TraceLog, Vector2, Rectangle
#include "gameplay/tiles.h"   // TILE_SIZE, TSPR_*

#define LINE_MAX 1024

// char -> tipo de tile (colisão)
static TileType tile_from_char(char c) {
    switch (c) {
        case '#': return TILE_SOLID;   // parede (colisão)
        case '.': return TILE_EMPTY;   // chão
        case 'P': return TILE_EMPTY;   // trilha (arte)
        case 'X': return TILE_EMPTY;   // túmulo (arte)
        case 'S': return TILE_EMPTY;   // spawn sobre chão
        case 'G': return TILE_EMPTY;   // goal sobre chão
        // traps como caracteres na grade não são sólidos
        case 'T': return TILE_EMPTY;   // trap padrão (compat)
        case '^': return TILE_EMPTY;   // spike explícito
        case 'F': return TILE_EMPTY;   // fogo
        case 'M': return TILE_EMPTY;   // mina
        case 'H': return TILE_EMPTY;   //oneway atravessavel
        case 'D': return TILE_EMPTY;   //bloco disappearing 
        default : return TILE_EMPTY;
    }
}

// char -> sprite index do atlas (arte)
static int sprite_index_from_char(char c) {
    switch (c) {
        case '#': return TSPR_WALL;
        case '.': return TSPR_FLOOR;
        case 'P': return TSPR_PATH;
        case 'X': return TSPR_GRAVE;
        case 'S': return TSPR_FLOOR;  // chão por baixo
        case 'G': return TSPR_GOAL;   // sprite de goal
        // traps como objeto: desenhamos piso por baixo
        case 'T': return TSPR_FLOOR;
        case '^': return TSPR_FLOOR;
        case 'F': return TSPR_FLOOR;
        case 'M': return TSPR_FLOOR;
        case 'H': return TSPR_FLOOR;
        case 'D': return TSPR_FLOOR;
        default : return TSPR_FLOOR;
    }
}

static inline void strip_newline(char *s) {
    if (!s) return;
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[--len] = '\0';
    }
}

bool level_loader_load(const char* path, Level* out) {
    if (!path || !out) {
        TraceLog(LOG_ERROR, "[level_loader] Parametros nulos.");
        return false;
    }

    // Defaults (tileSize, trapSet, etc.)
    level_init(out);

    // Limpa grade anterior (colisão + sprites) e metadados
    memset(out->tiles,   0, sizeof(out->tiles));
    memset(out->sprites, 0, sizeof(out->sprites));
    out->width  = 0;
    out->height = 0;
    out->spawn  = (Vector2){0,0};
    out->goal   = (Rectangle){0,0,0,0};
    trap_set_clear(&out->trapSet);

    FILE* f = fopen(path, "r");
    if (!f) {
        TraceLog(LOG_ERROR, "[level_loader] Nao abriu arquivo: %s", path);
        return false;
    }

    // 1ª linha útil: "W H"
    int width = 0, height = 0;
    char line[LINE_MAX];

    while (fgets(line, LINE_MAX, f)) {
        // Antes da grade, ignore ';', '#', e linhas vazias
        if (line[0] == ';' || line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        if (sscanf(line, "%d %d", &width, &height) == 2)
            break;
    }

    if (width <= 0 || height <= 0 ||
        width > LEVEL_MAX_WIDTH || height > LEVEL_MAX_HEIGHT) {
        TraceLog(LOG_ERROR,
                 "[level_loader] Dimensoes invalidas: %d x %d (max: %d x %d)",
                 width, height, LEVEL_MAX_WIDTH, LEVEL_MAX_HEIGHT);
        fclose(f);
        return false;
    }

    out->width  = width;
    out->height = height;

    const float tileSize = (out->tileSize > 0.0f) ? out->tileSize : (float)TILE_SIZE;

    Vector2   spawn = (Vector2){0};
    Rectangle goal  = (Rectangle){0};
    bool hasSpawn = false;
    bool hasGoal  = false;

    int y = 0;
    while (y < height && fgets(line, LINE_MAX, f)) {
        // Dentro da grade: ignore apenas linhas iniciadas por ';'
        if (line[0] == ';') continue;

        strip_newline(line);
        const int len = (int)strlen(line);

        for (int x = 0; x < width; x++) {
            const char c = (x < len) ? line[x] : '.';

            // Colisão + Sprite
            out->tiles[y][x]   = tile_from_char(c);
            out->sprites[y][x] = sprite_index_from_char(c);

            // Objetos/posições especiais
            switch (c) {
                case 'S':
                    // Centro do tile (x+0.5), base do pé (y+1.0) em PIXELS
                    spawn    = (Vector2){ (x + 0.5f) * tileSize, (y + 1.0f) * tileSize };
                    hasSpawn = true;
                    break;

                case 'G':
                    // goal como retângulo do tamanho do tile em PIXELS
                    goal     = (Rectangle){ x * tileSize, y * tileSize, tileSize, tileSize };
                    hasGoal  = true;
                    break;

                // traps como objetos (piso por baixo)
                case 'T':   // trap padrão (compat)
                case '^':   // spike explícito
                case 'F':   // fogo
                case 'M':   // mina
                case 'H':   //oneway atravessavel
                case 'D':{//disappearing 
                    Trap trap = (Trap){0};
                    trap.position = (Vector2){ (x + 0.5f) * tileSize, (y + 1.0f) * tileSize };
                    trap.hitbox   = (Rectangle){ x * tileSize, y * tileSize, tileSize, tileSize };

                    if (c == 'F')      trap.type = TRAP_TYPE_FALSE;        // F = Plataforma Falsa
                    else if (c == 'H') trap.type = TRAP_TYPE_ONEWAY;       // H = One-Way
                    else if (c == 'D') trap.type = TRAP_TYPE_DISAPPEARING; // D = Bloco que some
                    else if (c == 'M') trap.type = TRAP_TYPE_MINE;
                    else               trap.type = TRAP_TYPE_SPIKE;        // T ou ^
                    trap.active = true;
                    //Inicializar o estado padrão para o bloco D
                    trap.state = TRAP_STATE_ACTIVE;
                    trap.timer = 0.0f;

                    if (!trap_set_add(&out->trapSet, trap)) {
                        TraceLog(LOG_WARNING,
                                 "[level_loader] Trap descartada em (%d,%d) - Full", x, y);
                    }
                } break;

                default:
                    // '.', 'P', 'X' etc: só arte; já setamos colisão/sprite acima
                    break;
            }
        }
        y++;
    }
    fclose(f);

    if (y < height) {
        TraceLog(LOG_ERROR,
                 "[level_loader] Arquivo terminou antes da grade completa (%d/%d linhas).",
                 y, height);
        level_clear(out);
        return false;
    }

    out->spawn = spawn;
    out->goal  = goal;

    if (!hasSpawn) {
        TraceLog(LOG_WARNING, "[level_loader] Nao encontrou spawn (S) em %s", path);
    }
    if (!hasGoal) {
        TraceLog(LOG_WARNING, "[level_loader] Nao encontrou goal (G) em %s", path);
    }

    TraceLog(LOG_INFO, "[level_loader] OK %s (%dx%d) traps=%zu",
             path, width, height, out->trapSet.count);
    TraceLog(LOG_INFO, "[level_loader] spawn=(%.1f,%.1f) goal=(%.1f,%.1f)",
             out->spawn.x, out->spawn.y, out->goal.x, out->goal.y);

    return true;
}

void level_loader_unload(Level* level) {
    if (!level) return;
    level_clear(level);
}

bool level_loader_from_id(int levelId, Level* out) {
    if (!out) return false;
    char path[256];
    snprintf(path, sizeof(path), "data/levels/level%d.txt", levelId);
    return level_loader_load(path, out);
}
