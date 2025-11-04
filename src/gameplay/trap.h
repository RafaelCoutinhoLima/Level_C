#pragma once

#include <stddef.h>
#include <raylib.h>
#include <stdbool.h>

typedef enum TrapType{
    TRAP_TYPE_SPIKE = 0,
    TRAP_TYPE_FALLING,
    TRAP_TYPE_PROJECTILE,
    TRAP_TYPE_COUNT
}TrapType;

typedef struct Trap{
    Vector2 position;
    Rectangle hitbox;
    TrapType type;
    bool active;
}Trap;

#define TRAPSET_MAX_TRAPS 128

typedef struct TrapSet{
    Trap traps[TRAPSET_MAX_TRAPS];
    size_t count;
}TrapSet;

void trap_set_init(TrapSet* set);
bool trap_set_add(TrapSet* set, Trap trap);
void trap_set_clear(TrapSet* set);
const Trap* trap_set_get(const TrapSet* set, size_t index);
