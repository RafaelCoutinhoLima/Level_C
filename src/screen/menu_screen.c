#include "menu_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include <raylib.h>

static Button btnPlay;
static Button btnGameOver;

void menu_screen_init(void){
    TraceLog(LOG_INFO, "[Menu] init");
    btnPlay = CreateButton(-1, 260, 220, 45, "Jogar");
    btnGameOver = CreateButton(-1, 320, 220, 45, "Game Over");
}

void menu_screen_update(float dt){
    (void)dt;

    if (UpdateButton(&btnPlay)){
        state_change(SCREEN_PLAY);
    }else if (UpdateButton(&btnGameOver)){
        state_change(SCREEN_GAMEOVER);
    }
}

void menu_screen_draw(void){

    ClearBackground((Color){18, 18, 26, 255});

    const char *title = "MENU";
    const int fontSize = 48;
    const int textWidth = MeasureText(title, fontSize);

    DrawText(title, (GetScreenWidth() - textWidth) / 2, 140, fontSize, WHITE);
    DrawButton(btnPlay);
    DrawButton(btnGameOver);

}

void menu_screen_unload(void){
    TraceLog(LOG_INFO, "[Menu] unload");
}

GameState menu_screen_state(void){
    return (GameState){
        .init = menu_screen_init,
        .update = menu_screen_update,
        .draw = menu_screen_draw,
        .unload = menu_screen_unload,
    };
}