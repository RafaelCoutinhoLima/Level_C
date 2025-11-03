#ifndef SAVE_H
#define SAVE_H
#include <stdbool.h>

bool save_load_scores(const char* path);
bool save_write_score(const char* path, int score);

#endif
