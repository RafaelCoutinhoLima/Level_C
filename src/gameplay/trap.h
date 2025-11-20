#pragma once

#include <stddef.h>
#include <raylib.h>
#include <stdbool.h>

// Tipos de armadilhas (ordem preservada, adicionando FIRE e MINE)
typedef enum TrapType{
    TRAP_TYPE_SPIKE = 0,     // espinho (default)
    TRAP_TYPE_FIRE,          // NOVO: fogo
    TRAP_TYPE_MINE,          // NOVO: mina terrestre
    TRAP_TYPE_FALLING,       // queda (plataforma/objeto que cai)
    TRAP_TYPE_PROJECTILE,    // projétil
    TRAP_TYPE_COUNT
} TrapType;

typedef struct Trap{
    Vector2 position;   // centro do tile (em pixels)
    Rectangle hitbox;   // AABB em pixels
    TrapType type;
    bool active;
} Trap;

#define TRAPSET_MAX_TRAPS 128

typedef struct TrapSet{
    Trap traps[TRAPSET_MAX_TRAPS];
    size_t count;
} TrapSet;

// API do conjunto de traps
void trap_set_init(TrapSet* set);
bool trap_set_add(TrapSet* set, Trap trap);
void trap_set_clear(TrapSet* set);
const Trap* trap_set_get(const TrapSet* set, size_t index);
