#include "save.h"
#include <stdio.h>
#include <raylib.h>

bool save_load_scores(const char* path) {
    TraceLog(LOG_INFO, "[save] load %s (stub)", path ? path : "(null)");
    return true;
}

bool save_write_score(const char* path, int score) {
    TraceLog(LOG_INFO, "[save] write %s score=%d (stub)", path ? path : "(null)", score);
    return true;
}
