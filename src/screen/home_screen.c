#include "home_screen.h"
#include "raylib.h"
#include "core/state.h"//para as funções das mudanças 
#include "ui/button.h"

//criar uma static para o botão entrar
static Button btnGoToMenu;

//função para carregar a tela
void home_screen_init(void){
    TraceLog(LOG_INFO,"[Home] init");
    //usar x-1 para elee centralizar automaticamente
    //y=300,tam=250x50,txt:entrar
    btnGoToMenu = CreateButton(-1,300,250,50,"Entrar");

}

void home_screen_update(float dt){
    (void)dt;

    if (UpdateButton(&btnGoToMenu)){
        state_change(SCREEN_MENU);
        //se clicar pede a maquina estado mudar para tela screen menu
    }
}
void home_screen_draw(void){
    //limpa a tela com um cor de fundo azul escuro
    ClearBackground((Color){20, 20, 30, 255});

    const char *title = "LEVEL C";
    const int fontsize = 60;
    const int textWidth = MeasureText(title, fontsize);

    DrawText(title,(GetScreenWidth()-textWidth)/2, 150, fontsize, WHITE);
    DrawButton(btnGoToMenu);
}
void home_screen_unload(void){
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