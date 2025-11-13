#include "play_screen.h"
#include "levels/level_loader.h"
#include "io/input.h"
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "gameplay/physics.h"
#include "collisions/collision.h"
#include "gameplay/trap.h"
#include "core/state.h"
#include "ui/button.h"
#include "render/draw_utils.h"
#include <raylib.h>
#include "progress/progress.h"
#include "io/assets.h"
#include "render/draw_utils.h"

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;
static InputState gInput;
static Button btnBackToMenu;


void play_screen_init(void) {
    TraceLog(LOG_INFO, "[play] init");

    int level_to_load = progress_get_current_level();
    if (!level_loader_from_id(level_to_load, &gLevel)) {
        TraceLog(LOG_ERROR, "[play] falha ao carregar nivel %d",level_to_load);
        return;
    }

    // Player nasce no spawn em coordenadas de TILE.
    // Se o João usar pixels, converta: posPx = tile * TILE_SIZE
    player_init(&gPlayer);
    player_reset(&gPlayer, gLevel.spawn);
    collision_result_reset(&gCol);
    gInput = (InputState){0};
    
    input_init();
    physics_init(NULL);

    EnableCursor();
    btnBackToMenu = CreateButton(-1, GetScreenHeight() - 70, 220, 45, "Voltar ao menu");
}

void play_screen_update(float dt) {
    collision_result_reset(&gCol);

    input_update_player(&gInput);
    player_apply_input(&gPlayer, &gInput, dt);
    physics_update(&gPlayer, &gInput, dt);
    player_update_hitbox(&gPlayer);

    collisions_resolve_player_map(&gPlayer, &gLevel, &gCol);
    collisions_check_player_traps(&gPlayer, &gLevel.trapSet, &gCol);
    collisions_check_goal(&gPlayer, &gLevel.goal, &gCol);

    if (gCol.died) {
        TraceLog(LOG_INFO, "[Play] morreu -> reset level");
        player_reset(&gPlayer, gLevel.spawn);
        TraceLog(LOG_INFO, "[Play] ciclo: reset -> spawn -> limpeza de velocidade");
    }
    if (gCol.reachedGoal) {
        TraceLog(LOG_INFO, "[Play] chegou no goal -> completar progresso e trocar de tela");
        progress_complete_current_level();
        state_change(SCREEN_GAMEOVER);
        return;
    }
    if (UpdateButton(&btnBackToMenu)){
        TraceLog(LOG_INFO, "[Play] voltar ao menu");
        state_change(SCREEN_MENU);
        return;
    }
}

void play_screen_draw(void) {
    ClearBackground(RAYWHITE);
    
    draw_level_tiles(&gLevel);
    draw_traps(&gLevel.trapSet);
    draw_player(&gPlayer);
    draw_hud(&gPlayer,&gLevel,&gInput);
    DrawButton(btnBackToMenu);

}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
}

GameState play_screen_state(void){
    return (GameState){
        .init = play_screen_init,
        .update = play_screen_update,
        .draw = play_screen_draw,
        .unload = play_screen_unload,
    };
}