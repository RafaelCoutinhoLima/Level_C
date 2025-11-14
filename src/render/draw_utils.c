#include "render/draw_utils.h"
#include <raylib.h>
#include <stdio.h>
#include "gameplay/player.h"
#include <math.h>
#include "progress/progress.h"
#include "io/assets.h"

void DrawSprite(Rectangle source_rect,Vector2 position){
    Assets* assets = GetAssets();
    //os assets globais
    DrawTextureRec(assets->spritesheet_atlas,source_rect,position,WHITE);
}
void DrawSpriteAdvanced(Rectangle source_rect,Rectangle dest_rect,Color tint){
    Assets* assets= GetAssets();
    //definir a origem do desenho onde ele vai aparecer 
    Vector2 origin={0.0f, 0.0f };

    DrawTexturePro(assets->spritesheet_atlas,source_rect,dest_rect,origin,0.0f,tint);
}
void draw_level_tiles (const Level* level){
    if (!level){
        return;
    }
    for (int y=0;y<level->height;y++){
        for (int x=0;x<level->width;x++){
            if (!level_is_tile_solid(level,x,y)){
                continue;
            }
            Rectangle titleRect= level_tile_bounds(level,x,y);
            DrawRectangleRec(titleRect,(Color){60,200,80,100});
        }
    }
    DrawRectangleRec(level->goal,(Color){60,200,80,100});
}
void draw_traps(const TrapSet* trapSet){
    if (!trapSet){
        return;
    }
    for (size_t i=0;i<trapSet->count;i++){
        const Trap* trap=trap_set_get(trapSet,i);
        if (!trap||!trap->active){
            continue;
        }
        DrawRectangleRec(trap->hitbox,(Color){210,40,40,220});
        DrawRectangleLinesEx(trap->hitbox,1.5f,RED);
    }
}

void draw_player(const Player* player){
    if (!player) return;

    Assets* assets = GetAssets();
    if (assets->player_sheet.id == 0){
        Rectangle bounds = player_get_bounds(player);
        DrawRectangleRec(bounds, (Color){20, 20, 20, 255});
        return;
    }
    static float idleTimer = 0.0f;
    static int idleFrame = 0;

    idleTimer += GetFrameTime();
    if (idleTimer >= 1.0f / 6.0f){
        idleTimer = 0.0f;
        idleFrame = (idleFrame + 1) % 4; 
    }

    Rectangle src = {
        player->frameIndex * PLAYER_FRAME_WIDTH,
        player->animState * PLAYER_FRAME_HEIGHT,
        PLAYER_FRAME_WIDTH,
        PLAYER_FRAME_HEIGHT
    };

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
    if(!player->isAlive){
        return "MORTO";
    }//ve se ta vivo
    if(player->isOnGround){
        if(fabsf(player->velocity.x)<0.1f){
            return "PARADO";
        }else{
            return "CORRENDO";
        }
    }else{
        //se for negativo ta subindo,pulando
        if(player->velocity.y<0.0f){
            return "Pulando";
        }
        else{
            return "caindo";
        }
    }
    return "Desconhecido";
}
//tirando de playscreen usando level e player no lugar de gplaye glevel
void draw_hud(const Player* player, const Level* level,const InputState* input){
    if (!player || !level || !input) return;

    DrawText(TextFormat("pos(%.1f, %.1f) vel(%.1f, %.1f) traps=%zu", 
        player->position.x, player->position.y, 
        player->velocity.x, player->velocity.y, 
        level->trapSet.count), 
        12, 12, 18, LIGHTGRAY);
    DrawText(TextFormat("Estado: %s",GetPlayerStateName(player)),
    12,32,18,LIME);
    DrawText(TextFormat("Input: Eixo[%.1f] Pulo[%d]", input->moveAxis, input->jumpHeld),
        12, 52, 18, LIME);
        
    DrawText("ESC: voltar ao menu", 12, 76, 16, LIGHTGRAY);
    //mostrar o nivel atul
    DrawText(TextFormat("Nível: %d", progress_get_current_level()), 
        GetScreenWidth() - 100, 12, 18, DARKGRAY);
}