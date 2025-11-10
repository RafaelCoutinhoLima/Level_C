#include "input.h"
#include <raylib.h>

// Mantemos apenas leitura de teclado por enquanto.
// Mouse pode ser adicionado depois sem quebrar a API.

bool input_init(void) {
    // Nada assíncrono aqui, só garantimos estado inicial válido se preciso.
    return true;
}

void input_update_player(InputState* input) {
    if (!input) return;

    float axis = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) axis += 1.0f;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) axis -= 1.0f;

    input->moveAxis = axis;

    const bool jumpHeld    = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    const bool jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);

    input->jumpHeld    = jumpHeld;
    input->jumpPressed = jumpPressed;

    // Caso futuramente queira acelerar/andar devagar:
    input->runHeld = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
}
