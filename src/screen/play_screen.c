#include "levels/level_loader.h"
#include "io/input.h"
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "gameplay/physics.h"
#include "collisions/collision.h"

#include <raylib.h>

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;
static InputState gInput;

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

    collisions_resolve_player_map(&gPlayer, &gLevel, &gCol);
    collisions_check_player_traps(&gPlayer, &gLevel.trapSet, &gCol);
    collisions_check_goal(&gPlayer, &gLevel.goal, &gCol);

    if (gCol.died) {
        player_update_hitbox(&gPlayer);
        gPlayer.isOnGround = gCol.hitGround;

            TraceLog(LOG_INFO, "[play] morreu -> reset level");
            player_reset(&gPlayer, gLevel.spawn);
    }
    if (gCol.reachedGoal) {
        TraceLog(LOG_INFO, "[play] chegou no goal -> trocar de tela");
        // state_change(SCREEN_GAMEOVER); Rafa
    }
}

void play_screen_draw(void) {
    // Debug overlay mínimo:
    DrawText(TextFormat("pos(%.1f,%.1f) vel(%.1f,%.1f) traps=%zu",
        gPlayer.position.x, gPlayer.position.y,
        gPlayer.velocity.x, gPlayer.velocity.y,
        gLevel.trapSet.count), 8, 8, 16, WHITE);
}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
}
