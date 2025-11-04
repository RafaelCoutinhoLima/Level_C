#ifndef GAME_H
#define GAME_H
#include <stdbool.h>

//função para dar inicio do jogo
//retorna "true" se o jogo conseguir init
//caso falhe "falso" por isso booleano
bool game_init(void);

//aqui é loop do jogo (chama as funcionalidades da telas "update,draw") 
void game_loop(void);

//finaliza o jogo (descarrega o jogo e os recursos)
void game_shutdown(void);

#endif