#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <stdbool.h>
#include "gameplay/level.h"

// Convenção de nomes: implementa carregar/descarregar por path ou id.

bool level_loader_load(const char* path, Level* level);
void level_loader_unload(Level* level);

// Conveniência: carrega "data/levels/level%d.txt"
bool level_loader_from_id(int levelId, Level* out);

#endif // LEVEL_LOADER_H
