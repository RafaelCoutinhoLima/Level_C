#include "trap.h"
#include <raylib.h>

void trap_set_init(TrapSet* set){
    if (!set)
        return;
    
    set->count = 0;
    for (size_t i = 0; i<TRAPSET_MAX_TRAPS; i++){
        set->traps[i].active = false;
    }
    TraceLog(LOG_INFO, "[TrapSet] init");
}

bool trap_set_add(TrapSet* set, Trap trap){
    if (!set)
        return false;
    
    if (set->count >= TRAPSET_MAX_TRAPS){
        TraceLog(LOG_WARNING, "[TrapSet] overflox (max=%d)", TRAPSET_MAX_TRAPS);
        return false;
    }
    set->traps[set->count] = trap;
    set->traps[set->count].active = true;
    TraceLog(LOG_INFO, "[TrapSet] add -> idx=%zu type=%d pos=(%.1f, %.1f)", set->count, trap.type, trap.position.x, trap.position.y);

    set->count++;
    return true;
}

void trap_set_clear(TrapSet* set){
    if (!set)
        return;

    for (size_t i = 0; i<set->count; i++){
        set->traps[i].active = false;
    }
    set->count = 0;
    TraceLog(LOG_INFO, "[TrapSet] clear");
}

const Trap* trap_set_get(const TrapSet* set, size_t index){
    if (!set || index >= set->count){
        return NULL;
    }
    return &set->traps[index];
}

