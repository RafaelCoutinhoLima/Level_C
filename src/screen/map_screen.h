#ifndef MAP_SCREEN_H
#define MAP_SCREEN_H

#include "core/state.h"
#include "raylib.h"

#define  MAX_MAP_NODES 10
typedef enum {
    NODE_LOCKED,  
    NODE_AVAILABLE,
    NODE_COMPLETED,
}NodeState;

typedef struct {
    int levelId;//ID que vai ser passado para o level_loader
    Vector2 position;//posição x e y 
    NodeState state;//estado atual
    bool isHovered;
    float animScale; 
}LevelNode;
GameState map_screen_state(void);
#endif