#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <stdbool.h>
#include "gameplay/level.h"

// Carrega/descarrega nivel pelo caminho do arquivo.
// Retorna true em sucesso, false em erro (mensagens via TraceLog).
bool level_loader_load(const char* path, Level* level);
void level_loader_unload(Level* level);

// Conveniencia: carrega "data/levels/level%d.txt"
bool level_loader_from_id(int levelId, Level* out);

#endif // LEVEL_LOADER_H
