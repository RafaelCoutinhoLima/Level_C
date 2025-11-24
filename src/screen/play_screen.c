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
#include "io/audio.h"

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;
static InputState gInput;
static Button btnBackToMenu;
static Camera2D gCamera;

void play_screen_init(void) {
    TraceLog(LOG_INFO, "[play] init");

    audio_init();

    int level_to_load = progress_get_current_level();
    if (!level_load_by_id(&gLevel, level_to_load)) {
        TraceLog(LOG_ERROR, "[Play] Falha ao carregar nível %d, voltando ao mapa", level_to_load);
        state_change(SCREEN_MAP);
        return;
    }

    physics_init(NULL);
    physics_configure_for_level(&gLevel);

    player_init(&gPlayer);
    player_reset(&gPlayer, gLevel.spawn);

    collision_result_reset(&gCol);
    gInput = (InputState){0};
    input_init();

    EnableCursor();
    btnBackToMenu = CreateButton(-1, GetScreenHeight() - 70, 220, 45, "Voltar ao menu");

    float mapWidth  = gLevel.width  * gLevel.tileSize;
    float mapHeight = gLevel.height * gLevel.tileSize;
    gCamera.target  = (Vector2){ mapWidth / 2.0f, mapHeight / 2.0f };
    gCamera.offset  = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    gCamera.rotation= 0.0f;
    gCamera.zoom    = 1.3f;
}

void play_screen_update(float dt) {
    audio_update();

    collision_result_reset(&gCol);

    input_update_player(&gInput);
    player_apply_input(&gPlayer, &gInput, dt);
    physics_update(&gPlayer, &gInput, dt);
    player_update_hitbox(&gPlayer);
    physics_apply_level_bounds(&gPlayer, &gLevel);

    trap_set_update(&gLevel.trapSet, dt);
    collisions_resolve_player_map(&gPlayer, &gLevel, &gCol);
    collisions_check_player_traps(&gPlayer, &gLevel.trapSet, &gCol);
    collisions_check_goal(&gPlayer, &gLevel.goal, &gCol);

    player_anim_update(&gPlayer, dt);

    if (gCol.died) {
        audio_play_event(AUDIO_SFX_DIE);
        TraceLog(LOG_INFO, "[Play] morreu -> reset level");
        player_reset(&gPlayer, gLevel.spawn);
    }
    if (gCol.reachedGoal) {
        audio_play_event(AUDIO_SFX_GOAL);
        TraceLog(LOG_INFO, "[Play] chegou no goal -> completar progresso e trocar de tela");
        progress_complete_current_level();
        state_change(SCREEN_MAP);
        return;
    }
    if (UpdateButton(&btnBackToMenu)) {
        TraceLog(LOG_INFO, "[Play] voltar ao menu");
        state_change(SCREEN_MAP);
        return;
    }
}

void play_screen_draw(void) {
    ClearBackground(RAYWHITE);

    BeginMode2D(gCamera);
    draw_level_map(&gLevel);
    draw_level_tiles(&gLevel);
    draw_goal(&gLevel);
    draw_traps(&gLevel.trapSet);
    draw_player(&gPlayer);
    EndMode2D();

    draw_hud(&gPlayer, &gLevel, &gInput);
    DrawButton(btnBackToMenu);
}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
}

GameState play_screen_state(void) {
    return (GameState){
        .init   = play_screen_init,
        .update = play_screen_update,
        .draw   = play_screen_draw,
        .unload = play_screen_unload,
    };
}
