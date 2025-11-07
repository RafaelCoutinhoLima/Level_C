#include <raylib.h>

#include "screen/play_screen.h"
#include "screen/menu_screen.h"
#include "screen/home_screen.h"
#include "screen/gameover_screen.h"
#include "core/state.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Level C"

int main(void){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    state_init_manager();

    state_register(SCREEN_HOME, home_screen_state());
    state_register(SCREEN_MENU, menu_screen_state());
    state_register(SCREEN_PLAY, play_screen_state());
    state_register(SCREEN_GAMEOVER, gameover_screen_state());

    state_change(SCREEN_HOME);

    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        state_update(dt);
        state_draw();


    }
    state_shutdown();
    CloseWindow();
    return 0;
}