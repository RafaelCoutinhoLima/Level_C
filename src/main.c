#include "core/game.h" 
#include <stdlib.h> 
#include <raylib.h> 

int main(void) {
    
    if (!game_init()) {
        TraceLog(LOG_FATAL, "Falha ao inicializar o jogo.");
        game_shutdown(); 
        return EXIT_FAILURE;
    }

    game_loop();

    game_shutdown();

    return EXIT_SUCCESS;
}