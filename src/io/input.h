#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct InputState {
    float moveAxis;     // -1 (esquerda) .. 0 .. +1 (direita)
    bool  jumpHeld;     // tecla de pulo mantida
    bool  jumpPressed;  // borda de subida do pulo (1 frame)
    bool  runHeld;      
} InputState;
bool input_init(void);
void input_update_player(InputState* input);

#endif 
