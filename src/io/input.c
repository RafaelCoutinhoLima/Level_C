#include "input.h"
#include <raylib.h>
#include "gameplay/player.h" // precisa ter Player com velocity.x e algum "wantJump" opcional

bool input_init(void) {
    return true;
}

void input_update_player(InputState* input) {
    if (!input)
        return;

    float axis = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) axis += 1.0f;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) axis -= 1.0f;

    input->moveAxis = axis;

    bool jumpKeyDown = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool jumpKeyPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);

    input->jumpHeld = jumpKeyDown;
    input->jumpPressed = jumpKeyPressed;
}
