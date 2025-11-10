#include "assets.h"
#include <raylib.h>   // TraceLog, DirectoryExists, FileExists

static bool check_required_paths(void) {
    bool ok = true;

    // Diretorios/arquivos minimos do projeto
    if (!DirectoryExists("data")) {
        TraceLog(LOG_ERROR, "[assets] Diretório 'data' nao encontrado.");
        ok = false;
    }
    if (!DirectoryExists("data/levels")) {
        TraceLog(LOG_ERROR, "[assets] Diretório 'data/levels' nao encontrado.");
        ok = false;
    }

    // Opcional: verifique um nivel inicial padrao
    if (!FileExists("data/levels/level1.txt")) {
        TraceLog(LOG_WARNING, "[assets] Nao encontrei 'data/levels/level1.txt' (tudo bem se voce carregar outro).");
    }

    return ok;
}

bool assets_init(void) {
    TraceLog(LOG_INFO, "[assets] init");
    if (!check_required_paths()) {
        TraceLog(LOG_ERROR, "[assets] Estrutura minima de arquivos ausente.");
        return false;
    }

    // Futuro: carregar fontes/texturas/sons com LoadFont/LoadTexture/LoadSound...
    // Se falhar, dar TraceLog(LOG_ERROR, ...) e retornar false.

    return true;
}

void assets_unload(void) {
    TraceLog(LOG_INFO, "[assets] unload");
    // Futuro: UnloadTexture/UnloadFont/UnloadSound...
}
