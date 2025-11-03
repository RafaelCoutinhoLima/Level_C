#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

struct Player;

bool input_init(void);
void input_update_player(struct Player* p); // altera velocidade/flags conforme teclado

#endif
