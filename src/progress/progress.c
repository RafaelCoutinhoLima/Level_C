#include "progress.h"
#include <stdio.h> 
#include <raylib.h>

static int g_current_level_id = 1;      //Nível selecionado
static int g_max_unlocked_level = 1;    //Até onde o jogador chegou
static int g_total_deaths = 0;          //Contador de mortes

static const char *SAVE_FILE = "savegame.dat";

void progress_init(void) {
    FILE *file = fopen(SAVE_FILE, "r");
    
    if (file != NULL) {
        int read_count = fscanf(file, "%d %d", &g_max_unlocked_level, &g_total_deaths);

        if (read_count < 1) {
            g_max_unlocked_level = 1; // Arquivo vazio ou corrompido
            g_total_deaths = 0;
        }
        
        fclose(file);
        TraceLog(LOG_INFO, "[Progress] Save carregado. Max Level: %d, Mortes: %d", g_max_unlocked_level, g_total_deaths);
    } else {
        g_max_unlocked_level = 1; // Começa do zero
        g_total_deaths = 0;
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
        fprintf(file, "%d %d", g_max_unlocked_level, g_total_deaths);
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

void progress_complete_current_level(void) {
    // Só avança se eu completei a minha fase de anterior
    if (g_current_level_id == g_max_unlocked_level) {
        g_max_unlocked_level++;
        if (g_max_unlocked_level > MAX_LEVELS_SUPPORTED) {
            g_max_unlocked_level = MAX_LEVELS_SUPPORTED;
        }
        progress_save();
        TraceLog(LOG_INFO, "[Progress] Nova fase liberada: %d", g_max_unlocked_level);
    }
}

bool progress_is_level_completed(int level_id) {
    return level_id < g_max_unlocked_level;
}

void progress_add_death(void) {
    g_total_deaths++;
    progress_save(); // Salva a cada morte
}

int progress_get_total_deaths(void) {
    return g_total_deaths;
}
void progress_reset(void) {
    g_max_unlocked_level = 1;
    g_total_deaths = 0;
    progress_save();
    
    TraceLog(LOG_INFO, "[Progress] O save foi resetado completamente!");
}