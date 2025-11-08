#include "render/draw_utils.h"
#include <raylib.h>

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
    if(!player){
        return;
    }
    Rectangle bounds=player_get_bounds(player);
    DrawRectangleRec(bounds,(Color){20,20,20,255});
    if (!player->isAlive){
        DrawRectangleLinesEx(bounds,2.0f,RED);
    }
}
//tirando de playscreen usando level e player no lugar de gplaye glevel
void draw_hud(const Player* player, const Level* level){
    if (!player || !level) return;

    DrawText(TextFormat("pos(%.1f, %.1f) vel(%.1f, %.1f) traps=%zu", 
        player->position.x, player->position.y, 
        player->velocity.x, player->velocity.y, 
        level->trapSet.count), 
        12, 12, 18, LIGHTGRAY);
        
    DrawText("ESC: voltar ao menu", 12, 38, 16, LIGHTGRAY);
}