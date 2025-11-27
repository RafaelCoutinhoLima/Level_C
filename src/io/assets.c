#include "assets.h"
#include <raylib.h>   

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
    if (!FileExists("assets/tileset.png")) {
        TraceLog(LOG_ERROR, "[assets] Arquivo 'assets/tileset.png' nao encontrado (atlas obrigatorio).");
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

    //Carrega o atlas principal
    g_assets.spritesheet_atlas = LoadTexture("assets/tileset.png");
    if (g_assets.spritesheet_atlas.id == 0) {
        TraceLog(LOG_ERROR, "[assets] Falha ao carregar 'assets/tileset.png'");
        return false;
    }
    SetTextureFilter(g_assets.spritesheet_atlas, TEXTURE_FILTER_POINT);
    TraceLog(LOG_INFO, "[assets] tileset ok: %dx%d",
             g_assets.spritesheet_atlas.width, g_assets.spritesheet_atlas.height);

    //Player (opcional)
    if (FileExists("assets/player.png")) {
        g_assets.player_sheet = LoadTexture("assets/player.png");
        if (g_assets.player_sheet.id) {
            SetTextureFilter(g_assets.player_sheet, TEXTURE_FILTER_POINT);
            TraceLog(LOG_INFO, "[assets] player sheet id=%u", g_assets.player_sheet.id);
        } else {
            g_assets.player_sheet = (Texture2D){0};
        }
    } else {
        g_assets.player_sheet = (Texture2D){0};
    }

    //Carrega o fundo do mapa (map_bg.png)
    if (FileExists("assets/map_bg.png")) {
        g_assets.map_background = LoadTexture("assets/map_bg.png");
        if (g_assets.map_background.id) {
            SetTextureFilter(g_assets.map_background, TEXTURE_FILTER_POINT);
            TraceLog(LOG_INFO, "[assets] map_background carregado (map_bg.png).");
        } else {
            g_assets.map_background = (Texture2D){0};
        }
    } else {
        g_assets.map_background = (Texture2D){0};
    }

    //CARREGA A FONTE nova sem ser a da raylib padrão
    if (FileExists("assets/font.ttf")) {
        g_assets.gameFont = LoadFontEx("assets/font.ttf", 64, 0, 0);
        SetTextureFilter(g_assets.gameFont.texture, TEXTURE_FILTER_POINT);
        TraceLog(LOG_INFO, "[assets] Fonte carregada!");
    } else {
        TraceLog(LOG_WARNING, "[assets] font.ttf nao encontrada. Usando padrao.");
        g_assets.gameFont = GetFontDefault();
    }

    g_assets.rect_platform     = (Rectangle){  32, 0, 32, 32 };  
    g_assets.rect_trap_spike   = (Rectangle){ 64, 0, 32, 32 };  
    g_assets.rect_goal_door    = (Rectangle){ 96, 0, 32, 32 };  
    g_assets.rect_trap_false   = (Rectangle){ 128, 0, 32, 32 };
    g_assets.rect_spike_false = (Rectangle){64, 0, 32, 32};

    return true;
}

void assets_unload(void) {
    TraceLog(LOG_INFO, "[assets] unload");

    if (g_assets.spritesheet_atlas.id) {
        UnloadTexture(g_assets.spritesheet_atlas);
    }
    if (g_assets.player_sheet.id) {
        UnloadTexture(g_assets.player_sheet);
    }
    if (g_assets.map_background.id) {
        UnloadTexture(g_assets.map_background);
    }
    if (g_assets.gameFont.texture.id > 0) {
        UnloadFont(g_assets.gameFont);
    }
}