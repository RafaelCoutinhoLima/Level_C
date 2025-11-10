#include "level_loader.h"
#include "gameplay/level.h"
#include "gameplay/trap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>   // TraceLog, Vector2, Rectangle

#define LINE_MAX 1024

// Converte um caractere da grade do arquivo em um tipo de tile.
// Ajuste os valores conforme o que seu Level/tiles esperam.
static TileType tile_from_char(char c) {
    switch (c) {
        case '#': return 1; // sólido
        case '.': return 0; // vazio
        case 'S': return 0; // spawn (tile vazio)
        case 'G': return 0; // goal  (tile vazio)
        case 'T': return 0; // trap  (tile vazio, trap vai na lista)
        default : return 0; // desconhecido -> vazio (e podemos logar depois)
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

    level_clear(out);   // garante estado limpo antes
    level_init(out);    // inicializa defaults (tileSize, etc.)

    FILE* f = fopen(path, "r");
    if (!f) {
        TraceLog(LOG_ERROR, "[level_loader] Nao abriu arquivo de nivel: %s", path);
        return false;
    }

    // Formato suportado:
    //   1a linha util: "W H" (largura e altura)
    //   depois: H linhas com caracteres {#,.,S,G,T}
    //
    // Comentarios: linhas que comecam com ';' ou '#' fora da grade
    // sao ignoradas. Dentro da grade, cada linha deve possuir ao
    // menos W caracteres uteis (o que faltar vira '.').
    int width = 0, height = 0;
    char line[LINE_MAX];

    // Ler primeira linha com W H (pulando comentarios e linhas vazias)
    while (fgets(line, LINE_MAX, f)) {
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

    const float tileSize = (out->tileSize > 0.0f) ? out->tileSize : 32.0f;

    Vector2   spawn = (Vector2){0};
    Rectangle goal  = (Rectangle){0};
    bool hasSpawn = false;
    bool hasGoal  = false;

    int y = 0;
    while (y < height && fgets(line, LINE_MAX, f)) {
        // dentro da grade: apenas linha iniciada por ';' permanece ignorada
        if (line[0] == ';')
            continue;

        strip_newline(line);
        const int len = (int)strlen(line);

        for (int x = 0; x < width; x++) {
            const char c = (x < len) ? line[x] : '.';

            // Preenche tile
            const TileType t = tile_from_char(c);
            out->tiles[y][x] = t;

            // Spawn/Goal/Trap
            switch (c) {
                case 'S':
                    spawn    = (Vector2){ (x + 0.5f) * tileSize, (y + 1.0f) * tileSize };
                    hasSpawn = true;
                    break;
                case 'G':
                    goal     = (Rectangle){ x * tileSize, y * tileSize, tileSize, tileSize };
                    hasGoal  = true;
                    break;
                case 'T': {
                    Trap trap = {0};
                    trap.position = (Vector2){ (x + 0.5f) * tileSize, (y + 1.0f) * tileSize };
                    trap.hitbox   = (Rectangle){ x * tileSize, y * tileSize, tileSize, tileSize };
                    trap.type     = TRAP_TYPE_SPIKE;
                    trap.active   = true;

                    if (!trap_set_add(&out->trapSet, trap)) {
                        TraceLog(LOG_WARNING,
                                 "[level_loader] Capacidade de traps esgotada; descartando em (%d,%d)",
                                 x, y);
                    }
                } break;
                default:
                    // Se quiser logar tiles desconhecidos: TraceLog(LOG_DEBUG, ...)
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
        // sem spawn nao aborta: menu pode reposicionar manualmente, se quiser
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
