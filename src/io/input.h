#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

struct InputState;

bool input_init(void);
void input_update_player(struct InputState* input); // preenche o estado de input

#endif
