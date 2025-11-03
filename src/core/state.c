#include "core/state.h"
#include <stdlib.h>
#include <raylib.h>

#define MAX_STATES 20
//guardar todas as telas nesse array 
//e vai ser preenchida pela state_register
static GameState g_states[MAX_STATES];
//fazer um ponteiro apontar para a tela atual 
static GameState *g_current_state = NULL;
//para passar para a proxima tela usa -1 
//pq n tem nenhuma tela negativa é um valor inicial
static GameStateId g_next_state_id=-1;
//para salvar o id da tela atual -1 pelo mesmo 
//motivo    
static GameStateId g_current_state_id=-1;

void  state_init_manager(void){
    //zerar os ponteiros da função para garantir que n tenha lixo
    for (int i=0;i<MAX_STATES;i++){
        g_states[i]=(GameState){NULL ,NULL,NULL,NULL};
    }
    g_current_state=NULL;
    g_next_state_id=-1;
    g_current_state_id=-1;

    TraceLog(LOG_INFO,"State Manager inicializado.");
}