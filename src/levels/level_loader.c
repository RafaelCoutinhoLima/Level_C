#include "level_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>   // para TraceLog, Vector2, etc.
#include "gameplay/level.h"   // Level, Trap e afins (definidos pelo João)

#define LINE_MAX 1024

// Mapeia caracteres do arquivo para tiles (ajuste conforme seu projeto)
static int TileFromChar(char c) {
    switch (c) {
        case '#': return 1; // sólido
        case '.': return 0; // vazio
        case 'S': return 0; // spawn (tile vazio)
        case 'G': return 0; // goal  (tile vazio)
        case 'T': return 0; // trap  (tile vazio, trap vai em lista)
        default : return 0; // desconhecido vira vazio
    }
}

static void LevelZero(struct Level* L) {
    if (!L) return;
    memset(L, 0, sizeof(*L));
    L->tiles    = NULL;
    L->traps    = NULL;
    L->trapCount = 0;
}

static void FreeLevel(struct Level* L) {
    if (!L) return;
    if (L->tiles) free(L->tiles);
    if (L->traps) free(L->traps);
    L->tiles = NULL;
    L->traps = NULL;
    L->trapCount = 0;
}

bool level_loader_load(const char* path, struct Level* out) {
    if (!path || !out) return false;
    LevelZero(out);

    FILE* f = fopen(path, "r");
    if (!f) {
        TraceLog(LOG_ERROR, "[level_loader] Nao abriu arquivo: %s", path);
        return false;
    }

    // Formato suportado:
    // 1a linha: W H
    // depois: H linhas de grade com caracteres {#,.,S,G,T}
    // S = spawn, G = goal, T = trap, # = solido, . = vazio
    // Linhas iniciadas com ';' ou '#' como comentário são ignoradas fora da grade.

    int width = 0, height = 0;
    char line[LINE_MAX];

    // Ler primeira linha com W H (pulando comentários e vazias)
    while (fgets(line, LINE_MAX, f)) {
        if (line[0] == ';' || line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        if (sscanf(line, "%d %d", &width, &height) == 2) break;
    }

    if (width <= 0 || height <= 0) {
        TraceLog(LOG_ERROR, "[level_loader] Dimensoes invalidas em %s", path);
        fclose(f);
        return false;
    }

    int* tiles = (int*)calloc((size_t)width * (size_t)height, sizeof(int));
    if (!tiles) {
        TraceLog(LOG_ERROR, "[level_loader] Falha de memoria (tiles)");
        fclose(f);
        return false;
    }

    // Vamos armazenar traps numa lista temporária dinâmica
    typedef struct { int x, y; int type; } TrapTmp;
    TrapTmp* tmp = NULL;
    int tmpCount = 0, tmpCap = 0;

    Vector2 spawn = (Vector2){0,0};
    Vector2 goal  = (Vector2){0,0};
    bool hasSpawn = false, hasGoal = false;

    int y = 0;
    while (y < height && fgets(line, LINE_MAX, f)) {
        // Ignora linhas de comentário vazias, MAS aqui estamos dentro da grade,
        // então se a linha for curta, tratamos como vazia.
        if (line[0] == ';' || line[0] == '#') continue;

        // Garantir que a linha tem ao menos width caracteres úteis
        int len = (int)strlen(line);
        // remover \r\n
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        for (int x = 0; x < width; x++) {
            char c = (x < len) ? line[x] : '.';
            int tile = TileFromChar(c);
            tiles[y*width + x] = tile;

            if (c == 'S') {
                spawn = (Vector2){ (float)x, (float)y };
                hasSpawn = true;
            } else if (c == 'G') {
                goal = (Vector2){ (float)x, (float)y };
                hasGoal = true;
            } else if (c == 'T') {
                // push back
                if (tmpCount == tmpCap) {
                    tmpCap = tmpCap ? tmpCap*2 : 8;
                    TrapTmp* nt = (TrapTmp*)realloc(tmp, (size_t)tmpCap * sizeof(TrapTmp));
                    if (!nt) {
                        TraceLog(LOG_ERROR, "[level_loader] Falha de memoria (traps)");
                        free(tiles);
                        free(tmp);
                        fclose(f);
                        return false;
                    }
                    tmp = nt;
                }
                tmp[tmpCount++] = (TrapTmp){ x, y, 0 }; // type 0 generico por enquanto
            }
        }
        y++;
    }
    fclose(f);

    if (y < height) {
        TraceLog(LOG_ERROR, "[level_loader] Arquivo terminou antes da grade (%d/%d linhas)", y, height);
        free(tiles);
        free(tmp);
        return false;
    }

    // Copia traps temporárias para Level->traps (struct Trap do gameplay)
    Trap* traps = NULL;
    if (tmpCount > 0) {
        traps = (Trap*)calloc((size_t)tmpCount, sizeof(Trap));
        if (!traps) {
            TraceLog(LOG_ERROR, "[level_loader] Falha de memoria (Level->traps)");
            free(tiles);
            free(tmp);
            return false;
        }
        for (int i = 0; i < tmpCount; i++) {
            traps[i].pos = (Vector2){ (float)tmp[i].x, (float)tmp[i].y };
            traps[i].type = tmp[i].type;
            // ajuste: se Trap tiver bbox, defina aqui (ex: traps[i].hitbox = (Rectangle){...})
        }
    }
    free(tmp);

    // Preenche Level
    out->width  = width;
    out->height = height;
    out->tiles  = tiles;
    out->spawn  = spawn;
    out->goal   = goal;
    out->traps      = traps;
    out->trapCount  = tmpCount;

    if (!hasSpawn) TraceLog(LOG_WARNING, "[level_loader] Nao encontrou S (spawn) em %s", path);
    if (!hasGoal)  TraceLog(LOG_WARNING, "[level_loader] Nao encontrou G (goal) em %s", path);

    TraceLog(LOG_INFO, "[level_loader] OK %s  (%dx%d) traps=%d", path, width, height, tmpCount);
    TraceLog(LOG_INFO, "[level_loader] spawn=(%.0f,%.0f) goal=(%.0f,%.0f)",
             out->spawn.x, out->spawn.y, out->goal.x, out->goal.y);
    return true;
}

void level_loader_unload(struct Level* level) {
    if (!level) return;
    FreeLevel(level);
}

bool level_loader_from_id(int levelId, struct Level* out) {
    char path[256];
    snprintf(path, sizeof(path), "data/levels/level%d.txt", levelId);
    return level_loader_load(path, out);
}
