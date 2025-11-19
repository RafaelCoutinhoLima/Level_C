// src/render/draw_utils.c

#include "render/draw_utils.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "gameplay/player.h"
#include "gameplay/level.h"
#include "gameplay/tiles.h"   // TILE_SIZE, tileset_src_from_index
#include "io/input.h"         // InputState
#include "progress/progress.h"
#include "io/assets.h"

// -----------------------------------------------------------------------------
// Sprites helpers
// -----------------------------------------------------------------------------
void DrawSprite(Rectangle source_rect, Vector2 position){
    Assets* assets = GetAssets();
    DrawTextureRec(assets->spritesheet_atlas, source_rect, position, WHITE);
}

void DrawSpriteAdvanced(Rectangle source_rect, Rectangle dest_rect, Color tint){
    Assets* assets = GetAssets();
    Vector2 origin = (Vector2){0.0f, 0.0f};
    DrawTexturePro(assets->spritesheet_atlas, source_rect, dest_rect, origin, 0.0f, tint);
}

// -----------------------------------------------------------------------------
// Desenho do mapa via atlas (usa Level->sprites[y][x])
// -----------------------------------------------------------------------------
void draw_level_map(const Level* L) {
    if (!L) return;

    Assets* A = GetAssets();
    if (!A || !A->spritesheet_atlas.id) return;

    for (int y = 0; y < L->height; y++) {
        for (int x = 0; x < L->width; x++) {
            int idx = L->sprites[y][x];
            if (idx < 0) idx = 0; // fallback

            Rectangle src = tileset_src_from_index(idx);
            Vector2   dst = (Vector2){ x * (float)TILE_SIZE, y * (float)TILE_SIZE };
            DrawTextureRec(A->spritesheet_atlas, src, dst, WHITE);
        }
    }
}

// -----------------------------------------------------------------------------
// Sólidos (colisão). Usa rect_platform do atlas; se faltar, fallback colorido.
// -----------------------------------------------------------------------------
void draw_level_tiles (const Level* level){
    if (!level) return;

    Assets* A = GetAssets();
    float ts = level->tileSize;

    for (int y = 0; y < level->height; y++){
        for (int x = 0; x < level->width; x++){
            if (!level_is_tile_solid(level, x, y)) continue;

            Rectangle tileRect = level_tile_bounds(level, x, y);

            if (A && A->spritesheet_atlas.id && A->rect_platform.width > 0 && A->rect_platform.height > 0) {
                Vector2 pos = { tileRect.x, tileRect.y };
                DrawSprite(A->rect_platform, pos);
            } else {
                // Fallback visual
                DrawRectangleRec(tileRect, (Color){60,200,80,100});
                DrawRectangleLinesEx(tileRect, 1.0f, DARKGREEN);
            }
        }
    }

    // Goal como overlay simples (debug)
    DrawRectangleRec(level->goal,(Color){60,200,80,100});
}

// -----------------------------------------------------------------------------
// Traps (debug)
// -----------------------------------------------------------------------------
void draw_traps(const TrapSet* trapSet){
    if (!trapSet) return;

    for (size_t i = 0; i < trapSet->count; i++){
        const Trap* trap = trap_set_get(trapSet, i);
        if (!trap || !trap->active) continue;

        DrawRectangleRec(trap->hitbox,(Color){210,40,40,220});
        DrawRectangleLinesEx(trap->hitbox,1.5f,RED);
    }
}

// -----------------------------------------------------------------------------
// Player
// -----------------------------------------------------------------------------
void draw_player(const Player* player){
    if (!player) return;

    Assets* assets = GetAssets();
    if (assets->player_sheet.id == 0){
        Rectangle bounds = player_get_bounds(player);
        DrawRectangleRec(bounds, (Color){20, 20, 20, 255});
        return;
    }

    int absoluteFrame = player_get_absolute_frame(player);


    Rectangle src = {
        absoluteFrame * PLAYER_FRAME_WIDTH,
        0,
        PLAYER_FRAME_WIDTH,
        PLAYER_FRAME_HEIGHT
    };

    if (!player->facingRight){
        src.width = -src.width;
    }

    Vector2 drawPos = {
        player->hitbox.x + player->hitbox.width * 0.5f,
        player->hitbox.y + player->hitbox.height
    };

    Rectangle dest = {
        drawPos.x,
        drawPos.y,
        player->hitbox.width,
        player->hitbox.height
    };

    Vector2 origin = {
        player->hitbox.width * 0.5f,
        player->hitbox.height
    };

    DrawTexturePro(
        assets->player_sheet,
        src,
        dest,
        origin,
        0.0f,
        WHITE
    );
}

static const char* GetPlayerStateName(const Player* player){
    if(!player->isAlive) return "MORTO";

    if(player->isOnGround){
        if(fabsf(player->velocity.x) < 0.1f) return "PARADO";
        return "CORRENDO";
    }else{
        if(player->velocity.y < 0.0f) return "Pulando";
        return "caindo";
    }
}

// -----------------------------------------------------------------------------
// HUD (debug)
// -----------------------------------------------------------------------------
void draw_hud(const Player* player, const Level* level,const InputState* input){
    if (!player || !level || !input) return;

    DrawText(TextFormat("pos(%.1f, %.1f) vel(%.1f, %.1f) traps=%zu", 
        player->position.x, player->position.y, 
        player->velocity.x, player->velocity.y, 
        level->trapSet.count), 
        12, 12, 18, LIGHTGRAY);

    DrawText(TextFormat("Estado: %s", GetPlayerStateName(player)),
        12, 32, 18, LIME);

    DrawText(TextFormat("Input: Eixo[%.1f] Pulo[%d]", input->moveAxis, input->jumpHeld),
        12, 52, 18, LIME);
        
    DrawText("ESC: voltar ao menu", 12, 76, 16, LIGHTGRAY);

    DrawText(TextFormat("Nível: %d", progress_get_current_level()), 
        GetScreenWidth() - 100, 12, 18, DARKGRAY);
}
