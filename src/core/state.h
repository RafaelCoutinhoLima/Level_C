#ifndef STATE_H
#define STATE_H 
//para enumerar as telas do jogo o estado dele
typedef enum{
    SCREEN_HOME,
    SCREEN_MENU,
    SCREEN_PLAY,
    SCREEN_GAMEOVER,
} GameStateId;
// usa ponteiros para acessar as funções fica melhor
//por que senão teria que usar if else no state.c pode causar um bug
//sem saber necessariamente qual ta usando
typedef struct {
    void (*init)(void);
    void (*update)(void);
    void (*draw)(void);
    void (*unload)(void);
} GameState;

void state_init_manager(void);
void state_register(GameStateId id,GameState state);
void state_change(GameStateId next_id);

void state_update(void);
void state_draw(void);
void state_shutdown(void);

#endif
