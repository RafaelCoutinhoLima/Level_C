#include "play_screen.h"

#include "levels/level_loader.h"
#include "io/input.h"
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "gameplay/physics.h"
#include "collisions/collision.h"
#include "gameplay/trap.h"
#include "core/state.h"

#include <raylib.h>

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;
static InputState gInput;

static void draw_level_tiles(const Level* level);
static void draw_traps(const TrapSet* trapSet);
static void draw_player(const Player* player);
static void draw_hud(void);

void play_screen_init(void) {
    TraceLog(LOG_INFO, "[play] init");

    if (!level_loader_from_id(1, &gLevel)) {
        TraceLog(LOG_ERROR, "[play] falha ao carregar nivel 1");
        return;
        // opcional: fallback para sair
    }

    // Player nasce no spawn em coordenadas de TILE.
    // Se o João usar pixels, converta: posPx = tile * TILE_SIZE
    player_init(&gPlayer);
    player_reset(&gPlayer, gLevel.spawn);
    collision_result_reset(&gCol);
    gInput = (InputState){0};
    
    input_init();
    physics_init(NULL);
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
        TraceLog(LOG_INFO, "[play] morreu -> reset level");
        player_reset(&gPlayer, gLevel.spawn);
    }
    if (gCol.reachedGoal) {
        TraceLog(LOG_INFO, "[play] chegou no goal -> trocar de tela");
        state_change(SCREEN_GAMEOVER);
    }
    if (IsKeyPressed(KEY_ESCAPE)){
        state_change(SCREEN_MENU);
    }
}

void play_screen_draw(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_level_tiles(&gLevel);
    draw_traps(&gLevel.trapSet);
    draw_player(&gPlayer);
    draw_hud();

    EndDrawing();
}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
}

static void draw_level_tiles(const Level* level){
    if (!level)
        return;

    for (int y =0; y<level->height; y++){
        for (int x=0; x<level->width; x++){
            if (!level_is_tile_solid(level, x, y))
                continue;

            Rectangle tileRect = level_tile_bounds(level, x, y);
            DrawRectangleRec(tileRect, (Color){60, 200, 80, 180});
        }
    }
    DrawRectangleRec(level->goal, (Color){60, 200, 80, 180});
}

static void draw_traps(const TrapSet* trapSet){
    if (!trapSet)
        return;

    for (size_t i =0; i<trapSet->count; i++){
        const Trap* trap = trap_set_get(trapSet, i);
        if (!trap || !trap->active)
            continue;
        
        DrawRectangleRec(trap->hitbox, (Color){210, 40, 40, 220});
        DrawRectangleLinesEx(trap->hitbox, 1.5f, RED);
    }
}

static void draw_player(const Player* player){
    if (!player)
        return;

    Rectangle bounds = player_get_bounds(player);
    DrawRectangleRec(bounds, (Color){20, 20, 20, 255});

    if (!player->isAlive){
        DrawRectangleLinesEx(bounds, 2.0f, RED);
    }
}

static void draw_hud(void){
    DrawText(TextFormat("pos(%.1f, %.1f) vel(%.1f, %.1f) traps=%zu", gPlayer.position.x, gPlayer.position.y, gPlayer.velocity.x, gPlayer.velocity.y, gLevel.trapSet.count), 12, 12, 18, LIGHTGRAY);
    DrawText("ESC: voltar ao menu", 12, 38, 16, LIGHTGRAY);
}
