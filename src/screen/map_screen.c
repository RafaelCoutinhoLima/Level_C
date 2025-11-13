#include "map_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include <raylib.h>
#include "progress/progress.h"

static Button btnLevel1;
static Button btnLevel2;
static Button btnLevel3;
static Button btnLevel4;
static Button btnLevel5;

static Button btnBackToMenu;

void map_screen_init(void){
    TraceLog(LOG_INFO,"MAP init");
    btnLevel1 = CreateButton(-1,200,220,45,"Fase 1");
    btnLevel2 = CreateButton(-1,260,220,45,"Fase 2");
    btnLevel3 = CreateButton(-1,320,220,45,"Fase 3");
    btnLevel4 = CreateButton(-1,380,220,45,"Fase 4");
    btnLevel5 = CreateButton(-1,440,220,45,"Fase 5");
    btnBackToMenu = CreateButton(-1,500,220,45,"Voltar ao Menu");   
}
void map_screen_update(float dt){
    (void)dt;
    if (UpdateButton(&btnLevel1)){
        progress_set_current_level(1);
        state_change(SCREEN_PLAY);
    }
    if (UpdateButton(&btnLevel2)){
        progress_set_current_level(2);
        state_change(SCREEN_PLAY);
    }
    if (UpdateButton(&btnLevel3)){
        progress_set_current_level(3);
        state_change(SCREEN_PLAY);
    }
    if (UpdateButton(&btnLevel4)){
        progress_set_current_level(4);
        state_change(SCREEN_PLAY);
    }
    if (UpdateButton(&btnLevel5)){
        progress_set_current_level(5);
        state_change(SCREEN_PLAY);
    }
    if (UpdateButton(&btnBackToMenu)){
        state_change(SCREEN_MENU);
    }
}

void map_screen_draw(void){
    ClearBackground((Color){20,30,20,255});

    const char *title = "SELEÇÃO DE FASES";
    const int fontsize = 48;
    const int  textWidth=MeasureText(title,fontsize);

    DrawText(title,(GetScreenWidth()-textWidth)/2,100,fontsize,WHITE);

    DrawButton(btnLevel1);
    DrawButton(btnLevel2);
    DrawButton(btnLevel3);
    DrawButton(btnLevel4);
    DrawButton(btnLevel5);
    DrawButton(btnBackToMenu);
}

void map_screen_unload(void){
    TraceLog(LOG_INFO, "[Map] unload");
}

GameState map_screen_state(void){
    return (GameState){
        .init = map_screen_init,
        .update = map_screen_update,
        .draw = map_screen_draw,
        .unload = map_screen_unload,
    };
}