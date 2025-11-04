#include "level_loader.h"
#include "gameplay/level.h"
#include "gameplay/trap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>   // para TraceLog, Vector2, etc.

#define LINE_MAX 1024

// Mapeia caracteres do arquivo para tiles (ajuste conforme seu projeto)
static TileType tile_from_char(char c) {
    switch (c) {
        case '#':
            return 1; // sólido
        case '.':
            return 0; // vazio
        case 'S':
            return 0; // spawn (tile vazio)
        case 'G':
            return 0; // goal  (tile vazio)
        case 'T':
            return 0; // trap  (tile vazio, trap vai em lista)
        default :
            return 0; // desconhecido vira vazio
    }
}

bool level_loader_load(const char* path, Level* out) {
    if (!path || !out)
        return false;
    
    level_init(out);

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

    int width = 0;
    int height = 0;
    char line[LINE_MAX];

    // Ler primeira linha com W H (pulando comentários e vazias)
    while (fgets(line, LINE_MAX, f)) {
        if (line[0] == ';' || line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        if (sscanf(line, "%d %d", &width, &height) == 2)
            break;
    }

    if (width <= 0 || height <= 0 || width > LEVEL_MAX_WIDTH || height > LEVEL_MAX_HEIGHT) {
        TraceLog(LOG_ERROR, "[level_loader] Dimensoes invalidas (%d x %d)", width, height, path);
        fclose(f);
        return false;
    }

    out->width = width;
    out->height = height;

    const float tileSize = out->tileSize > 0.0f ? out->tileSize : 32.0f;    

    Vector2 spawn = (Vector2){0};
    Rectangle goal  = (Rectangle){0};
    bool hasSpawn = false;
    bool hasGoal = false;

    int y = 0;
    while (y < height && fgets(line, LINE_MAX, f)) {
        // Ignora linhas de comentário vazias, MAS aqui estamos dentro da grade,
        // então se a linha for curta, tratamos como vazia.
        if (line[0] == ';' || line[0] == '#') 
            continue;
        // Garantir que a linha tem ao menos width caracteres úteis
        int len = (int)strlen(line);
        // remover \r\n
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        for (int x = 0; x < width; x++) {
            const char c = (x < len) ? line[x] : '.';
            out->tiles[x][y] = tile_from_char(c);

            if (c == 'S') {
                spawn = (Vector2){(x + 0.5f) * tileSize,(y + 1.0f) * tileSize};
                hasSpawn = true;
            } else if (c == 'G') {
                goal = (Rectangle){x * tileSize, y * tileSize, tileSize, tileSize};
                hasGoal = true;
            } else if (c == 'T') {
                // push back
                Trap trap = {0};
                trap.position = (Vector2){(x + 0.5f) * tileSize, (y + 1.0f) * tileSize};
                trap.hitbox = (Rectangle){x * tileSize, y * tileSize, tileSize, tileSize};
                trap.type = TRAP_TYPE_SPIKE;
                trap.active = true;

                if (!trap_set_add(&out->trapSet, trap)){
                    TraceLog(LOG_WARNING, "[level_loader] Trap overflow descartando trap (%d,%d)", x, y);
                }
            }
        }
        y++;
    }
    fclose(f);

    if (y < height) {
        TraceLog(LOG_ERROR, "[level_loader] Arquivo terminou antes da grade (%d/%d linhas)", y, height);
        level_clear(out);
        return false;
    }

    out->spawn = spawn;
    out->goal = goal;

    if (!hasSpawn)
        TraceLog(LOG_WARNING, "[level_loader] Não encontrou spawn em %s", path);

    if (!hasGoal)
        TraceLog(LOG_WARNING, "[level_loader] Não encontrou goal em %s", path);
    
    TraceLog(LOG_INFO, "[level_loader] OK %s (%dx%d) traps= %zu", path, width, height, out->trapSet.count);
    TraceLog(LOG_INFO, "[level_loader] spawn=(%.1f,%.1f) goal=(%.1f,%.1f)", out->spawn.x, out->spawn.y, out->goal.x, out->goal.y);

    return true;
}

void level_loader_unload(Level* level) {
    if (!level)
        return;
    level_clear(level);
}

bool level_loader_from_id(int levelId, Level* out) {
    char path[256];
    snprintf(path, sizeof(path), "data/levels/level%d.txt", levelId);
    return level_loader_load(path, out);
}
