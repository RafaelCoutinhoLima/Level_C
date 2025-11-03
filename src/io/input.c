#include "input.h"
#include <raylib.h>
#include "gameplay/player.h" // precisa ter Player com velocity.x e algum "wantJump" opcional

bool input_init(void) {
    return true;
}

void input_update_player(struct Player* p) {
    if (!p) return;

    float ax = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) ax += 1.0f;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) ax -= 1.0f;

    // Integra com físicas do João: aqui só sinalizamos intenção.
    p->inputX = ax; // se o Player não tiver, João pode ler velocity.x direto ou criar esse campo

    // Pulo opcional: setar um flag que a física consome (edge-trigger)
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        p->wantJump = true; // campo boolean no Player (consumido pela physics_update)
    }
}
