#include "game.h"
#include "core/state.h"
#include <raylib.h>
#include <stdbool.h>

//mudei da main para ca os includes 
#include"screen/play_screen.h"
#include"screen/menu_screen.h"
#include"screen/home_screen.h"
#include"screen/gameover_screen.h"
#include "screen/map_screen.h"
#include "progress/progress.h"
#include "io/input.h"

//constante para o jogo a janela
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Level C Game"
#define TARGET_FPS 60

bool game_init(void){
    //Configura a flag de Janela Maximizada ANTES de iniciar a janela
    // A janela será iniciada com o tamanho máximo da área de trabalho,
    //e deixa uma janela aberta
    // mantendo a barra de tarefas visível.
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED);

    //incializar a janela com a engine do raylib
    //com as resoluções screen_width e height

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,GAME_TITLE);

    if (!IsWindowReady()){
        TraceLog(LOG_FATAL,"RAYLIB:Falha ao inicializar a janela");
        return false;
    }
    //essa função vai fazer com que a pesso a possa desmaximizar a janela
    //e arrastar ela 
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);
    //iniciar o estado do jogo se ta no menu na home 
    //essas coisas
    state_init_manager();
    progress_init();
    input_init();
    //mudei a logica da main para aqui fica mais clean a main
    state_register(SCREEN_HOME,home_screen_state());
    state_register(SCREEN_MENU,menu_screen_state());
    state_register(SCREEN_MAP,map_screen_state());
    state_register(SCREEN_PLAY,play_screen_state());
    state_register(SCREEN_GAMEOVER,gameover_screen_state());
    
    state_change(SCREEN_HOME);
    TraceLog(LOG_INFO,"Jogo inicializado em Janela Maximizada");
    return true;
}
void game_loop(void){
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        state_update(dt);

        BeginDrawing();
        //Para limpar a tela
        ClearBackground(RAYWHITE);

        state_draw();
        EndDrawing();
    }
}
void game_shutdown(void){
    state_shutdown();
    CloseWindow();
    TraceLog(LOG_INFO,"jogo finalizado");
}