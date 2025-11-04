#include "game.h"
#include "core/state.h"
#include <raylib.h>
#include <stdbool.h>

//constante para o jogo a janela

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Level C Game"
#define TARGET_FPS 60

bool game_init(void){
    //iniciar a tela dependendo do tamanho da tela
    int monitor =GetCurrentMonitor();
    int full_width=GetMonitorWidth(monitor);
    int full_height=GetMonitorHeight(monitor); 
    //Configura a flag de Janela Maximizada ANTES de iniciar a janela
    // A janela será iniciada com o tamanho máximo da área de trabalho,
    //e deixa uma janela aberta
    // mantendo a barra de tarefas visível.
    SetWindowState(FLAG_WINDOW_MAXIMIZED);

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
    TraceLog(LOG_INFO,"Jogo inicializado em Janela Maximizada");
    return true;
}