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
static GameStateId g_next_state_id=STATE_ID_NONE;
//para salvar o id da tela atual -1 que é o state none pelo mesmo 
//motivo    
static GameStateId g_current_state_id=STATE_ID_NONE;

void state_init_manager(void){
    //zerar os ponteiros da função para garantir que n tenha lixo
    for (int i=0;i<MAX_STATES;i++){
        g_states[i]=(GameState){NULL ,NULL,NULL,NULL};
    }
    g_current_state=NULL;
    g_next_state_id=STATE_ID_NONE;
    g_current_state_id=STATE_ID_NONE;

    TraceLog(LOG_INFO,"State Manager inicializado.");
}
void state_change(GameStateId next_id){
    //validação do id como se fosse uma flag
    if (next_id < 0 || next_id>= MAX_STATES){
        TraceLog(LOG_ERROR, "state_change: id inválido (%d).", next_id);
        return;
    }
    if (g_states[next_id].init == NULL && g_states[next_id].update == NULL && g_states[next_id].draw == NULL && g_states[next_id].unload == NULL){
        TraceLog(LOG_ERROR, "state_change: estado %d não foi registrado.", next_id);
        return;
    }
    g_next_state_id = next_id;
}

static void do_state_change(void){
    //faz uma verificação se a flag não mudou ou seja a tela 
    if (g_next_state_id==STATE_ID_NONE){
        return;
    }
    if (g_current_state!=NULL && g_current_state->unload!=NULL){
        g_current_state->unload();
        //faz o unload da tela atual que é a "passada"
    }
    g_current_state_id=g_next_state_id;
    g_current_state=&g_states[g_current_state_id];
    g_next_state_id=STATE_ID_NONE;
    //faz o swap das telas e atualiza a proxima para -1 que é o state none


    //inicializar a nova tela e verifica se ela tem init
    if (g_current_state!=NULL && g_current_state->init !=NULL){
        g_current_state->init();
        TraceLog(LOG_INFO,"Estado mudado para %d", g_current_state_id);
    }else{
        TraceLog(LOG_ERROR,"Falha ao iniciar estado %d:não tem init",g_current_state_id);
    }
}

void state_update(float dt){
    //antes de dar o update chamo para ver se vai trocar de tela
    do_state_change();
    //fazer o update agora g_current já ta na tela certa
    if (g_current_state!=NULL && g_current_state->update!=NULL){
        g_current_state->update(dt);
    }
}
void state_draw(void){
    //roda o desenho na tela current
    if (g_current_state!=NULL && g_current_state->draw!=NULL){
        g_current_state->draw();
    }
}
void state_shutdown(void){
    //descarrega a ultima tela 
    if (g_current_state!=NULL && g_current_state->unload !=NULL){
        g_current_state->unload();
    }
    TraceLog(LOG_INFO,"State Manager finalizado");
}

void state_register(GameStateId id, GameState state){
    if (id < 0 || id>= MAX_STATES){
        TraceLog(LOG_ERROR, "Tentativa de registrar estado inválido (%d).", id);
        return;
    }

    g_states[id] = state;
    TraceLog(LOG_INFO, "Estado %d registrado (init=%p, update=%p, draw=%p, unload=%p)", id, (void *)state.init, (void *)state.update, (void *)state.draw, (void *)state.unload);
}
