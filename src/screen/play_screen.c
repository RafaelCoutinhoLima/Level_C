#include "levels/level_loader.h"
#include "io/input.h"
#include "gameplay/level.h"
#include "gameplay/player.h"
#include "collisions/collision.h"
#include <raylib.h>

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;

void play_screen_init(void) {
    TraceLog(LOG_INFO, "[play] init");

    if (!level_loader_from_id(1, &gLevel)) {
        TraceLog(LOG_ERROR, "[play] falha ao carregar nivel 1");
        // opcional: fallback para sair
    }

    // Player nasce no spawn em coordenadas de TILE.
    // Se o João usar pixels, converta: posPx = tile * TILE_SIZE
    gPlayer.position = (Vector2){ gLevel.spawn.x, gLevel.spawn.y };
    gPlayer.velocity = (Vector2){ 0, 0 };
    gPlayer.isAlive = true;
    gPlayer.wantJump = false;
    gPlayer.inputX = 0;

    input_init();
}

void play_screen_update(float dt) {
    memset(&gCol, 0, sizeof(gCol));

    input_update_player(&gPlayer);         // <- tua parte
    physics_update(&gPlayer, dt);          // <- do João
    collisions_resolve_player_map(&gLevel, &gPlayer, &gCol);
    collisions_check_player_traps(&gLevel, &gPlayer, &gCol);
    collisions_check_goal(&gLevel, &gPlayer, &gCol);

    if (gCol.hitTrap || gCol.died) {
        TraceLog(LOG_INFO, "[play] morreu -> reset level");
        gPlayer.position = (Vector2){ gLevel.spawn.x, gLevel.spawn.y };
        gPlayer.velocity = (Vector2){ 0, 0 };
        // (ou chamar player_reset/level_reset_player quando existir)
    }
    if (gCol.reachedGoal) {
        TraceLog(LOG_INFO, "[play] chegou no goal -> trocar de tela");
        // state_change(SCREEN_GAMEOVER); // Rafael cuida disso
    }
}

void play_screen_draw(void) {
    // Debug overlay mínimo:
    DrawText(TextFormat("pos(%.1f,%.1f) vel(%.1f,%.1f) traps=%d",
        gPlayer.position.x, gPlayer.position.y,
        gPlayer.velocity.x, gPlayer.velocity.y,
        gLevel.trapCount), 8, 8, 16, WHITE);
}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
}
