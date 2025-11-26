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
#include "effects/particle.h"
#include <math.h>

static Level gLevel;
static Player gPlayer;
static CollisionResult gCol;
static InputState gInput;
static Button btnBackToMenu;
static Camera2D gCamera;
static ParticleSystem gParticles;

static bool gGoalReached = false;
static float gGoalTimer = 0.0f;
static const float GOAL_DELAY = 1.0f;

void play_screen_init(void) {
    TraceLog(LOG_INFO, "[play] init");

    audio_init(); 
    audio_stop_music();
    
    gGoalReached = false;
    gGoalTimer = 0.0f;

    int level_to_load = progress_get_current_level();
    if (!level_load_by_id(&gLevel, level_to_load)) {
        TraceLog(LOG_ERROR, "[Play] Falha ao carregar nível %d", level_to_load);
        audio_play_music(); 
        state_change(SCREEN_MAP);
        return;
    }

    physics_init(NULL);
    physics_configure_for_level(&gLevel);

    player_init(&gPlayer);
    player_reset(&gPlayer, gLevel.spawn);
    particle_system_init(&gParticles);

    collision_result_reset(&gCol);
    gInput = (InputState){0};
    input_init();

    EnableCursor();

    int btnW = 220;
    int btnH = 45;
    int btnX = (GetScreenWidth() - btnW) / 2; 
    
    btnBackToMenu = CreateButton(btnX, GetScreenHeight() - 70, btnW, btnH, "Voltar ao menu");

    float mapWidth  = gLevel.width  * gLevel.tileSize;
    float mapHeight = gLevel.height * gLevel.tileSize;
    gCamera.target  = (Vector2){ mapWidth / 2.0f, mapHeight / 2.0f };
    gCamera.offset  = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    gCamera.rotation= 0.0f;
    gCamera.zoom    = 1.3f; 
}

void play_screen_update(float dt) {
    // Se já alcançou o goal, só conta o timer
    if (gGoalReached) {
        gGoalTimer += dt;
        particle_system_update(&gParticles, dt);
        
        if (gGoalTimer >= GOAL_DELAY) {
            audio_play_music();
            state_change(SCREEN_MAP);
            return;
        }
        return;
    }
    
    audio_update();

    collision_result_reset(&gCol);
    input_update_player(&gInput);

    player_apply_input(&gPlayer, &gInput, dt);
    physics_update(&gPlayer, &gInput, dt);
    player_update_hitbox(&gPlayer);
    physics_apply_level_bounds(&gPlayer, &gLevel);
    particle_system_update(&gParticles, dt);

    trap_set_update(&gLevel.trapSet);

    collisions_resolve_player_map(&gPlayer, &gLevel, &gCol);
    collisions_check_player_traps(&gPlayer, &gLevel.trapSet, &gCol);
    collisions_check_goal(&gPlayer, &gLevel.goal, &gCol);

    player_anim_update(&gPlayer, dt);

    if (gCol.died) {
        audio_play_event(AUDIO_SFX_DIE);
        particle_emit_explosion(&gParticles, gPlayer.position, 25);
        progress_add_death();
        trap_set_reset(&gLevel.trapSet);
        player_reset(&gPlayer, gLevel.spawn);
    }

    if (gCol.reachedGoal) {
        TraceLog(LOG_INFO, "[Play] GOAL! Mostrando animação...");
        
        particle_emit_goal(&gParticles, gPlayer.position);
        audio_play_event(AUDIO_SFX_GOAL);
        progress_complete_current_level();
        
        gGoalReached = true;
        gGoalTimer = 0.0f;
        return;  // ← SÓ ISSO! Não chama state_change aqui
    }

    if (gPlayer.isOnGround && fabsf(gPlayer.velocity.x) > 50.0f) {
        static float dustTimer = 0.0f;
        dustTimer += dt;
        
        if (dustTimer >= 0.1f) {  // A cada 0.1 segundos
            Vector2 dustPos = (Vector2){
                gPlayer.position.x,
                gPlayer.position.y + gPlayer.hitbox.height / 2  // Embaixo do player
            };
            particle_emit_dust(&gParticles, dustPos);
            dustTimer = 0.0f;
        }
    }

    if (UpdateButton(&btnBackToMenu)) {
        audio_play_music(); 
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
    particle_system_draw(&gParticles);
    EndMode2D();
    
    Rectangle r = btnBackToMenu.bounds;
    bool hover = btnBackToMenu.hovered;

    Color outlineCol = (Color){ 25, 25, 45, 255 }; 
    Color mainCol    = hover ? (Color){ 80, 220, 240, 255 } : (Color){ 0, 190, 210, 255 }; 
    Color shadowCol  = (Color){ 0, 140, 160, 255 }; 
    DrawRectangleRec((Rectangle){r.x + 4, r.y + 4, r.width, r.height}, outlineCol);

    DrawRectangleRec(r, outlineCol);
    DrawRectangleRec((Rectangle){r.x + 3, r.y + 3, r.width - 6, r.height - 6}, mainCol);
    
    DrawRectangleRec((Rectangle){r.x + 3, r.y + r.height - 10, r.width - 6, 7}, shadowCol);

    int fontSize = 20;
    int textWidth = MeasureText(btnBackToMenu.text, fontSize);
    int textX = r.x + (r.width - textWidth) / 2;
    int textY = r.y + (r.height - fontSize) / 2;
    DrawText(btnBackToMenu.text, textX, textY, fontSize, outlineCol);
}

void play_screen_unload(void) {
    level_loader_unload(&gLevel);
    particle_system_clear(&gParticles);
}

GameState play_screen_state(void) {
    return (GameState){
        .init   = play_screen_init,
        .update = play_screen_update,
        .draw   = play_screen_draw,
        .unload = play_screen_unload,
    };
}