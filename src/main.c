#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector2 position;   // posição (pixels) do canto superior esquerdo
    Vector2 velocity;   // velocidade (px/s)
    float width, height;    // largura e altura (pixels)
    int facing;    // 1 = direita, -1 = esquerda (útil depois p/ sprite)
    bool onGround; // está no chão?
} Player;

int main(void) {
    // 1) Janela e timing
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Level C");
    SetTargetFPS(60);

    // 2) Player placeholder (retângulo)
    Player player = {0};
    player.position = (Vector2){100, 300};
    player.velocity = (Vector2){0, 0};
    player.width = 24;
    player.height = 32;
    player.facing = 1;
    player.onGround = false;

    // 3) Física básica
    const float gravity   = 1500.0f; // px/s^2 (ajuste para “peso” desejado)
    const float moveSpeed = 240.0f;  // px/s
    const float jumpSpeed = -420.0f; // px/s (negativo = para cima)

    // 4) Chão fixo (placeholder): um retângulo no fundo
    Rectangle ground = (Rectangle){ 0, 380, (float)screenWidth, 70 };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 5) Input horizontal
        player.velocity.x = 0;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  { player.velocity.x = -moveSpeed; player.facing = -1; }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { player.velocity.x =  moveSpeed; player.facing =  1; }

        // 6) Gravidade
        player.velocity.y += gravity * dt;

        // 7) Pulo (só se estiver no chão)
        if (player.onGround && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))) {
            player.velocity.y = jumpSpeed;
            player.onGround = false;
        }

        // 8) Atualiza posição
        player.position.x += player.velocity.x * dt;
        player.position.y += player.velocity.y * dt;

        // 9) Checar colisão com o chão (e resolver)
        // Base do player:
        float playerBottom = player.position.y + player.height;
        if (playerBottom >= ground.y) {
            // Se “entrou” no chão e está caindo, empurra para cima
            if (player.velocity.y >= 0) {
                player.position.y = ground.y - player.height;
                player.velocity.y = 0;
                player.onGround = true;
            }
        } else {
            player.onGround = false;
        }

        // 10) Limites horizontais da janela
        if (player.position.x < 0) player.position.x = 0;
        if (player.position.x + player.width > screenWidth) player.position.x = screenWidth - player.width;

        // 11) Desenho
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Chão
            DrawRectangleRec(ground, ORANGE);

            // Player
            DrawRectangle((int)player.position.x, (int)player.position.y, (int)player.width, (int)player.height, BLACK);

            // HUD simples
            DrawText("A/D ou setas: mover | SPACE/W/UP: pular | ESC: sair", 10, 10, 18, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}