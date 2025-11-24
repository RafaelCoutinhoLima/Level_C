#include "game.h"
#include "core/state.h"
#include <raylib.h>
#include <stdbool.h>

#include "screen/play_screen.h"
#include "screen/home_screen.h"
#include "screen/map_screen.h"
#include "progress/progress.h"
#include "io/input.h"
#include "io/assets.h"
#include "io/audio.h"

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE    "Level C Game"
#define TARGET_FPS    60

bool game_init(void) {
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);

    if (!IsWindowReady()) {
        TraceLog(LOG_FATAL, "RAYLIB: Falha ao inicializar a janela");
        return false;
    }
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);

    if (!assets_init()) {
        TraceLog(LOG_FATAL, "[game] falha ao inicializar assets");
        return false;
    }
    // Áudio não é obrigatório para iniciar, mas tentamos
    audio_init();

    state_init_manager();
    progress_init();
    input_init();

    state_register(SCREEN_HOME, home_screen_state());
    state_register(SCREEN_MAP,  map_screen_state());
    state_register(SCREEN_PLAY, play_screen_state());

    state_change(SCREEN_HOME);
    TraceLog(LOG_INFO, "Jogo inicializado em Janela Maximizada");
    return true;
}

void game_loop(void) {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        state_update(dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        state_draw();
        EndDrawing();
    }
}

void game_shutdown(void) {
    audio_shutdown();
    assets_unload();
    state_shutdown();
    CloseWindow();
    TraceLog(LOG_INFO, "jogo finalizado");
}
