#include "home_screen.h"
#include "raylib.h"
#include "core/state.h"
#include "ui/button.h"
#include "progress/progress.h"
static Button btnGoToMenu;
static Texture2D titleScreen;


static Rectangle sourceButtonRec = { 460, 500, 420, 120 };

void home_screen_init(void){
    TraceLog(LOG_INFO,"[Home] init");
    titleScreen = LoadTexture("assets/title-screen.png");
    
    // Mantém o pixel art nítido
    SetTextureFilter(titleScreen, TEXTURE_FILTER_POINT); 

    btnGoToMenu = CreateButton(sourceButtonRec.x, sourceButtonRec.y, sourceButtonRec.width, sourceButtonRec.height, "");
}

void home_screen_update(float dt){
    (void)dt;
    if (UpdateButton(&btnGoToMenu)){
        state_change(SCREEN_MAP);
    }
    //resetar o save (Tecla R)
    if (IsKeyPressed(KEY_R)) {
        progress_reset();
    }
}
void home_screen_draw(void){
    ClearBackground((Color){20, 20, 30, 255});
    DrawTexture(titleScreen, 0, 0, WHITE);

    if (btnGoToMenu.hovered) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        float scale = 1.03f; 
        
        Rectangle destRec = {
            sourceButtonRec.x - (sourceButtonRec.width * (scale - 1.0f)) / 2.0f,
            sourceButtonRec.y - (sourceButtonRec.height * (scale - 1.0f)) / 2.0f - 2.0f,
            sourceButtonRec.width * scale,
            sourceButtonRec.height * scale
        };

        DrawTexturePro(
            titleScreen,
            sourceButtonRec,
            destRec,
            (Vector2){0,0}, 
            0.0f, 
            WHITE
        );

    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void home_screen_unload(void){
    UnloadTexture(titleScreen);
    TraceLog(LOG_INFO,"[Home] unload");
}

GameState home_screen_state(void){
    return (GameState){
        .init = home_screen_init,
        .update = home_screen_update,
        .draw = home_screen_draw,
        .unload = home_screen_unload,
    };
}