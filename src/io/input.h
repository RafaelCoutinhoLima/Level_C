#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

// Estado de input desacoplado do Player.
// O jogo lê este struct e aplica na física do jogador.
typedef struct InputState {
    float moveAxis;     // -1 (esquerda) .. 0 .. +1 (direita)
    bool  jumpHeld;     // tecla de pulo mantida
    bool  jumpPressed;  // borda de subida do pulo (1 frame)
    bool  runHeld;      // opcional: corrida (shift)
} InputState;

// Inicializa sistema de input (se no futuro precisarmos configurar algo).
bool input_init(void);

// Atualiza o estado de input de jogador a cada frame/tick.
void input_update_player(InputState* input);

#endif // INPUT_H
