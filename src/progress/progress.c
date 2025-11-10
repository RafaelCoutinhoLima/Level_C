#include "progress.h"
#include <raylib.h>

// Guardamos o ID do nível que o jogador quer jogar.
// Começa em 1 por padrão.
static int g_current_level_id = 1;
static bool g_levels_completed[MAX_LEVELS_SUPPORTED];
void progress_init(void){
    g_current_level_id=1;
    for (int i=0;i<MAX_LEVELS_SUPPORTED;i++){
        g_levels_completed[i]=false;
    }
    TraceLog(LOG_INFO,"PROGRESS ,sistema de progresso inicializado");
}
void progress_set_current_level(int level_id) {
    if (level_id <=0 || level_id >MAX_LEVELS_SUPPORTED) {
        TraceLog(LOG_WARNING,"tentativa de setar o nivel inválida: %d",level_id);
        level_id = 1; // Garante que nunca seja um nível inválido
    }
    g_current_level_id = level_id;
}

int progress_get_current_level(void) {
    return g_current_level_id;
}
//Implementação de 'completar nível'
void progress_complete_current_level(void) {
    int level_to_complete = g_current_level_id;

    if (level_to_complete <= 0 || level_to_complete > MAX_LEVELS_SUPPORTED) {
        TraceLog(LOG_ERROR, "[Progress] Tentativa de completar nível inválido: %d", level_to_complete);
        return;
    }

    // Marca o nível como completo (usando índice 0-based)
    int index = level_to_complete - 1;
    g_levels_completed[index] = true;

    // Esta é a linha que cumpre o seu requisito de validação!
    TraceLog(LOG_INFO, "[Progress] Nível %d marcado como completo.", level_to_complete);
}

// Implementação de 'checar nível'
bool progress_is_level_completed(int level_id) {
    if (level_id <= 0 || level_id > MAX_LEVELS_SUPPORTED) {
        return false;
    }
    
    int index = level_id - 1;
    return g_levels_completed[index];
}