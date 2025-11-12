#include "assets.h"
#include <raylib.h>   // TraceLog, DirectoryExists, FileExists

static Assets g_assets;
Assets* GetAssets(void){
    return &g_assets;
}
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
    if (!FileExists("resources/atlas.png")) {
        TraceLog(LOG_ERROR, "[assets] Spritesheet 'resources/atlas.png' nao encontrado.");
        ok = false;
    }

    return ok;
}

bool assets_init(void) {
    TraceLog(LOG_INFO, "[assets] init");
    if (!check_required_paths()) {
        TraceLog(LOG_ERROR, "[assets] Estrutura minima de arquivos ausente.");
        return false;
    }
    g_assets.spritesheet_atlas = LoadTexture("resources/atlas.png");
    if (g_assets.spritesheet_atlas.id == 0) {
        TraceLog(LOG_ERROR, "[assets] Falha ao carregar 'resources/atlas.png'");
        return false;
    }

    g_assets.rect_player     = (Rectangle){ 0, 0, 32, 32 };
    g_assets.rect_platform   = (Rectangle){ 32, 0, 32, 32 };
    g_assets.rect_trap_spike = (Rectangle){ 64, 0, 32, 32 };
    g_assets.rect_goal_door  = (Rectangle){ 96, 0, 32, 32 };
    g_assets.rect_button_play= (Rectangle){ 0, 32, 128, 64 };
    // Futuro: carregar fontes/texturas/sons com LoadFont/LoadTexture/LoadSound...
    return true;
}

void assets_unload(void) {
    TraceLog(LOG_INFO, "[assets] unload");
    UnloadTexture(g_assets.spritesheet_atlas);
}
