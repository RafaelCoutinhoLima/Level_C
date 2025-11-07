#include "gameover_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include <raylib.h>

static Button btnBackToMenu;

void gameover_screen_init(void){
    TraceLog(LOG_INFO, "[GameOver] init");
    btnBackToMenu = CreateButton(-1, 320, 260, 45, "Voltar ao menu");
}

void gameover_screen_update(float dt){
    (void)dt;

    if(UpdateButton(&btnBackToMenu)){
        state_change(SCREEN_MENU);
    }
}

void gameover_screen_draw(void){
    BeginDrawing();

    ClearBackground((Color){32, 0, 0, 255});

    const char *title = "GAME OVER";
    const int fontSize = 48;
    const int textWidth = MeasureText(title, fontSize);

    DrawText(title, (GetScreenWidth() - textWidth) / 2, 140, fontSize, RED);
    DrawButton(btnBackToMenu);

    EndDrawing();
}

void gameover_screen_unload(void){
    TraceLog(LOG_INFO, "[GameOver] unload");
}

GameState gameover_screen_state(void){
    return (GameState){
        .init = gameover_screen_init,
        .update = gameover_screen_update,
        .draw = gameover_screen_draw,
        .unload = gameover_screen_unload,
    };
}