#include "assets.h"
#include <raylib.h>

bool assets_init(void) {
    TraceLog(LOG_INFO, "[assets] init (stub)");
    // futuramente: LoadTexture/LoadFont/etc.
    return true;
}

void assets_unload(void) {
    TraceLog(LOG_INFO, "[assets] unload (stub)");
    // futuramente: UnloadTexture/UnloadFont/etc.
}
