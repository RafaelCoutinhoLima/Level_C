#include "render/draw_utils.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "gameplay/player.h"
#include "gameplay/level.h"
#include "gameplay/tiles.h"   
#include "io/input.h"         
#include "progress/progress.h"
#include "io/assets.h"

void draw_level_map(const Level* L) {
    if (!L) return;

    Assets* A = GetAssets();
    if (!A || !A->spritesheet_atlas.id) return;

    for (int y = 0; y < L->height; y++) {
        for (int x = 0; x < L->width; x++) {
            int idx = L->sprites[y][x];
            if (idx < 0) idx = 0; 

            Rectangle src = tileset_src_from_index(idx);
            Vector2   dst = (Vector2){ x * (float)TILE_SIZE, y * (float)TILE_SIZE };
            DrawTextureRec(A->spritesheet_atlas, src, dst, WHITE);
        }
    }
}
void draw_level_tiles (const Level* level){
    if (!level) return;

    Assets* A = GetAssets();

    for (int y = 0; y < level->height; y++){
        for (int x = 0; x < level->width; x++){
            if (!level_is_tile_solid(level, x, y)) continue;

            Rectangle tileRect = level_tile_bounds(level, x, y);

            if (A && A->spritesheet_atlas.id && A->rect_platform.width > 0 && A->rect_platform.height > 0) {
                Vector2 pos = { tileRect.x, tileRect.y };
                DrawTextureRec(A->spritesheet_atlas, A->rect_platform, pos, WHITE);
            } else {
                // Fallback visual
                DrawRectangleRec(tileRect, (Color){60,200,80,100});
                DrawRectangleLinesEx(tileRect, 1.0f, DARKGREEN);
            }
        }
    }
}
// Traps (Aqui está a mágica da Trap Falsa)
void draw_traps(const TrapSet* trapSet){
    if (!trapSet) return;

    for (size_t i = 0; i < trapSet->count; i++){
        const Trap* t = trap_set_get(trapSet, i);
        if (!t || !t->active) continue;

        Assets* A = GetAssets();

        switch (t->type) {
            //F: Desenha a trap FALSA exatamente igual ao ESPINHO verdadeiro
            case TRAP_TYPE_SPIKE:
            case TRAP_TYPE_FALSE: 
                DrawTexturePro(
                    A->spritesheet_atlas,
                    A->rect_trap_spike,
                    t->hitbox,
                    (Vector2){0, 0},
                    0.0f,
                    WHITE
                );
                break;
                
            //H:One-Way / Atravessável (Azul)
            case TRAP_TYPE_ONEWAY:
                DrawRectangle(t->hitbox.x, t->hitbox.y, t->hitbox.width, 6, BLACK);
                break;

            //D:Bloco que Some (Laranja)
            case TRAP_TYPE_DISAPPEARING:
                if (t->state == TRAP_STATE_OFF) {
                } else {
                    DrawTexturePro(
                        A->spritesheet_atlas,
                        A->rect_trap_false,
                        t->hitbox,
                        (Vector2){0, 0},
                        0.0f,
                        WHITE
                    );
                }
                break;
            default:
                DrawRectangleRec(t->hitbox, GRAY);
                break;
        }
    }
}
// Player
void draw_player(const Player* player){
    if (!player) return;

    Assets* assets = GetAssets();
    // Caso não consiga carregar a sprite, gera um personagem genérico
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
// Goal
void draw_goal(const Level* level) {
    if (!level) return;
    
    Assets* A = GetAssets();
    
    if (A && A->spritesheet_atlas.id && A->rect_goal_door.width > 0 && A->rect_goal_door.height > 0) {
        DrawTexturePro(
            A->spritesheet_atlas,
            A->rect_goal_door,
            level->goal,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    }
}