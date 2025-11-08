#include "core/game.h" // <--- O único include que ele precisa
#include <stdlib.h> // Para EXIT_SUCCESS / EXIT_FAILURE
#include <raylib.h> // Para TraceLog, LOG_FATAL (opcional mas bom)

int main(void) {
    
    // 1Tenta inicializar o jogo
    // (game_init() agora faz TUDO: InitWindow, state_init, state_register)
    if (!game_init()) {
        TraceLog(LOG_FATAL, "Falha ao inicializar o jogo.");
        game_shutdown(); // Tenta fechar o que quer que tenha sido aberto
        return EXIT_FAILURE;
    }

    //Roda o loop principal do jogo
    // (game_loop() agora tem o "while", state_update, Begin/EndDrawing, state_draw)
    game_loop();

    //Desliga o jogo
    // (game_shutdown() agora faz o state_shutdown e CloseWindow)
    game_shutdown();

    return EXIT_SUCCESS;
}