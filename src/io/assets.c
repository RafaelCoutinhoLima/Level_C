#include "assets.h"
#include <raylib.h>   // TraceLog, DirectoryExists, FileExists

static Assets g_assets;

Assets* GetAssets(void) {
    return &g_assets;
}

static bool check_required_paths(void) {
    bool ok = true;

    if (!DirectoryExists("data")) {
        TraceLog(LOG_ERROR, "[assets] Diretório 'data' nao encontrado.");
        ok = false;
    }
    if (!DirectoryExists("data/levels")) {
        TraceLog(LOG_ERROR, "[assets] Diretório 'data/levels' nao encontrado.");
        ok = false;
    }
    if (!DirectoryExists("assets")) {
        TraceLog(LOG_ERROR, "[assets] Diretório 'assets' nao encontrado.");
        ok = false;
    }
    // Atlas do tileset é obrigatório na Sprint 3
    if (!FileExists("assets/tileset.png")) {
        TraceLog(LOG_ERROR, "[assets] Arquivo 'assets/tileset.png' nao encontrado (atlas obrigatorio).");
        ok = false;
    }
    // Player é opcional (aviso apenas)
    if (!FileExists("assets/player.png")) {
        TraceLog(LOG_WARNING, "[assets] 'assets/player.png' nao encontrado (seguindo sem player sheet).");
    }

    return ok;
}

bool assets_init(void) {
    TraceLog(LOG_INFO, "[assets] init");

    if (!check_required_paths()) {
        TraceLog(LOG_ERROR, "[assets] Estrutura minima de arquivos ausente.");
        return false;
    }

    // Carrega o atlas principal
    g_assets.spritesheet_atlas = LoadTexture("assets/tileset.png");
    if (g_assets.spritesheet_atlas.id == 0) {
        TraceLog(LOG_ERROR, "[assets] Falha ao carregar 'assets/tileset.png'");
        return false;
    }
    SetTextureFilter(g_assets.spritesheet_atlas, TEXTURE_FILTER_POINT);
    TraceLog(LOG_INFO, "[assets] tileset ok: %dx%d",
             g_assets.spritesheet_atlas.width, g_assets.spritesheet_atlas.height);

    // Player (opcional)
    if (FileExists("assets/player.png")) {
        g_assets.player_sheet = LoadTexture("assets/player.png");
        if (g_assets.player_sheet.id) {
            SetTextureFilter(g_assets.player_sheet, TEXTURE_FILTER_POINT);
            TraceLog(LOG_INFO, "[assets] player sheet id=%u", g_assets.player_sheet.id);
        } else {
            TraceLog(LOG_WARNING, "[assets] Falha ao carregar 'assets/player.png' (seguindo sem).");
            g_assets.player_sheet = (Texture2D){0};
        }
    } else {
        g_assets.player_sheet = (Texture2D){0};
    }
// 3.Carrega o fundo do mapa (map_bg.png)
    if (FileExists("assets/map_bg.png")) {
        g_assets.map_background = LoadTexture("assets/map_bg.png");
        if (g_assets.map_background.id) {
            // Se a imagem for pixel art, mantemos o filtro POINT. 
            // Se for alta resolução, pode remover esta linha.
            SetTextureFilter(g_assets.map_background, TEXTURE_FILTER_POINT);
            TraceLog(LOG_INFO, "[assets] map_background carregado (map_bg.png).");
        } else {
            TraceLog(LOG_WARNING, "[assets] Falha ao carregar 'assets/map_bg.png'.");
            g_assets.map_background = (Texture2D){0};
        }
    } else {
        TraceLog(LOG_WARNING, "[assets] 'assets/map_bg.png' nao encontrado. O mapa ficara sem fundo.");
        g_assets.map_background = (Texture2D){0};
    }

    // Definição dos Retângulos do Tileset (Convenção)
    g_assets.rect_platform     = (Rectangle){  32, 0, 32, 32 };  // (0,1)
<<<<<<< HEAD
    g_assets.rect_trap_spike   = (Rectangle){ 192, 0, 32, 32 };  // (0,6)
    g_assets.rect_goal_door    = (Rectangle){ 160, 0, 32, 32 };  // (0,5)
    g_assets.rect_button_play  = (Rectangle){ 128, 0, 32, 32 };  // (0,4)
=======
    g_assets.rect_trap_spike   = (Rectangle){ 64, 0, 32, 32 };  // (0,6)
    g_assets.rect_goal_door    = (Rectangle){ 96, 0, 32, 32 };  // (0,5)
    g_assets.rect_trap_false  = (Rectangle){ 128, 0, 32, 32 };
>>>>>>> 6e10015 (Tiles sendo desenhadas corretamente)

    return true;
}

void assets_unload(void) {
    TraceLog(LOG_INFO, "[assets] unload");

    if (g_assets.spritesheet_atlas.id) {
        UnloadTexture(g_assets.spritesheet_atlas);
        g_assets.spritesheet_atlas = (Texture2D){0};
    }
    if (g_assets.player_sheet.id) {
        UnloadTexture(g_assets.player_sheet);
        g_assets.player_sheet = (Texture2D){0};
    }
    if (g_assets.map_background.id) {
        UnloadTexture(g_assets.map_background);
        g_assets.map_background = (Texture2D){0};
    }
}
