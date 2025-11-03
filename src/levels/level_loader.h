#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <stdbool.h>

struct Level; // forward declaration para evitar conflito
// Convenção de nomes: implementa carregar/descarregar por path ou id.

bool level_loader_load(const char* path, struct Level* out);
void level_loader_unload(struct Level* level);

// Conveniência: carrega "data/levels/level%d.txt"
bool level_loader_from_id(int levelId, struct Level* out);

#endif // LEVEL_LOADER_H
