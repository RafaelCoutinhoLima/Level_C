#pragma once

#include <stddef.h>
#include <raylib.h>
#include <stdbool.h>

// Tipos de armadilhas (ordem preservada, adicionando FIRE e MINE)
typedef enum TrapType{
    TRAP_TYPE_SPIKE = 0,     // espinho (default)
    TRAP_TYPE_ONEWAY,        // H:atravesavel de baixo para cima
    TRAP_TYPE_FALSE,         // F:plataforma falsa
    TRAP_TYPE_DISAPPEARING,
} TrapType;
typedef enum TrapState{
    TRAP_STATE_ACTIVE,
    TRAP_STATE_OFF,
}TrapState;
typedef struct Trap{
    Vector2 position;   // centro do tile (em pixels)
    Rectangle hitbox;   // AABB em pixels
    TrapType type;
    bool active;
    TrapState state;
    float timer; //para os blocos temporarios D
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
//pra saber que o bloco D foi pisado por exemplo alterar o estado dele
Trap* trap_set_get_mutable(TrapSet* set,size_t index);
const Trap* trap_set_get(const TrapSet* set, size_t index);
//para atualizar o timer 
void trap_set_update(TrapSet* set,float dt);
void trap_set_reset(TrapSet* set);
