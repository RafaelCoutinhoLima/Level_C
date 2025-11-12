#ifndef ASSETS_H
#define ASSETS_H

#include <stdbool.h>
#include <raylib.h>

//estrutura para o assets carregados
typedef struct Assets {
    Texture2D spritesheet_atlas;
    //retangulo de fontes
    // Em assets.h
    Rectangle rect_player;       // O boneco
    Rectangle rect_platform;     // O bloco de chão/parede sólido
    Rectangle rect_trap_spike;   // Um espinho
    Rectangle rect_goal_door;    // A "porta" para passar de nível
    Rectangle rect_button_play;  // O botão de "jogar" do menu
}Assets;

// Inicializa/verifica assets e estrutura minima de pastas.
// Retorna false se algo critico estiver faltando.
bool assets_init(void);

// Descarta recursos carregados (texturas, fontes, sons...), se houver.
void assets_unload(void);

//getter para acessar carregado em qualquer lugar

Assets *GetAssets(void);
#endif // ASSETS_H
