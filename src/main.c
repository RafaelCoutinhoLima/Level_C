#include <raylib.h>

#include "screen/play_screen.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Level C"

int main(void){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    play_screen_init();

    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        play_screen_update(dt);
        play_screen_draw();

    }

    play_screen_unload();
    CloseWindow();
    return 0;
}