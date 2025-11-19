#ifndef MAP_SCREEN_H
#define MAP_SCREEN_H

#include "core/state.h"
#include "raylib.h"

#define  MAX_MAP_NODES 10
typedef enum {
    NODE_LOCKED,  //bloquear fase
    NODE_AVAILABLE,//fase disponivel
    NODE_COMPLETED,//fase concluido 
}NodeState;

typedef struct {
    int levelId;//ID que vai ser passado para o level_loader
    Vector2 position;//posição x e y 
    NodeState state;//estado atual
    bool isHovered;//se o mouse ta em cima
    float animScale; //a animação pular
}LevelNode;
GameState map_screen_state(void);
#endif