#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <stdbool.h>
#include "gameplay/level.h"

bool level_loader_load(const char* path, Level* level);
void level_loader_unload(Level* level);

bool level_loader_from_id(int levelId, Level* out);

#endif 
