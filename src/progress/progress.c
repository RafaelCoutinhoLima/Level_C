#include "progress.h"

// Guardamos o ID do nível que o jogador quer jogar.
// Começa em 1 por padrão.
static int g_current_level_id = 1;

void progress_set_current_level(int level_id) {
    if (level_id < 1) {
        level_id = 1; // Garante que nunca seja um nível inválido
    }
    g_current_level_id = level_id;
}

int progress_get_current_level(void) {
    return g_current_level_id;
}