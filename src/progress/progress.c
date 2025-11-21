#include "progress.h"
#include <stdio.h>  // Necessário para FILE, fopen, etc.
#include <raylib.h>

static int g_current_level_id = 1;      // Nível selecionado
static int g_max_unlocked_level = 1;    // Até onde o jogador chegou
static const char *SAVE_FILE = "savegame.dat";

void progress_init(void) {
    // Tenta carregar o save do arquivo
    FILE *file = fopen(SAVE_FILE, "r");
    
    if (file != NULL) {
        if (fscanf(file, "%d", &g_max_unlocked_level) != 1) {
            g_max_unlocked_level = 1; // Erro na leitura, reseta
        }
        fclose(file);
        TraceLog(LOG_INFO, "[Progress] Save carregado. Max Level: %d", g_max_unlocked_level);
    } else {
        g_max_unlocked_level = 1; // Começa do zero
        TraceLog(LOG_INFO, "[Progress] Novo jogo iniciado.");
    }
    // Garante que não ultrapasse o limite
    if (g_max_unlocked_level > MAX_LEVELS_SUPPORTED) {
        g_max_unlocked_level = MAX_LEVELS_SUPPORTED;
    }
}

void progress_save(void) {
    FILE *file = fopen(SAVE_FILE, "w");
    if (file != NULL) {
        fprintf(file, "%d", g_max_unlocked_level);
        fclose(file);
        TraceLog(LOG_INFO, "[Progress] Jogo salvo com sucesso!");
    } else {
        TraceLog(LOG_WARNING, "[Progress] Erro ao escrever o save.");
    }
}

void progress_set_current_level(int level_id) {
    if (level_id <= 0 || level_id > MAX_LEVELS_SUPPORTED) {
        TraceLog(LOG_WARNING, "[Progress] Nivel invalido: %d", level_id);
        level_id = 1;
    }
    g_current_level_id = level_id;
}

int progress_get_current_level(void) {
    return g_current_level_id;
}

int progress_get_max_unlocked(void) {
    return g_max_unlocked_level;
}

// Chama isso quando tocar no PORTAL
void progress_complete_current_level(void) {
    // Só avança se eu completei a minha fase de "anterior"
    if (g_current_level_id == g_max_unlocked_level) {
        g_max_unlocked_level++;
        // Trava no máximo
        if (g_max_unlocked_level > MAX_LEVELS_SUPPORTED) {
            g_max_unlocked_level = MAX_LEVELS_SUPPORTED;
        }
        progress_save(); // Salva imediatamente
        TraceLog(LOG_INFO, "[Progress] Nova fase liberada: %d", g_max_unlocked_level);
    }
}

bool progress_is_level_completed(int level_id) {
    // Se o nível perguntado for menor que o meu máximo, eu já completei
    return level_id < g_max_unlocked_level;
}