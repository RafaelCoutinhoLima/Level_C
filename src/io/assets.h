#ifndef ASSETS_H
#define ASSETS_H

#include <stdbool.h>
#include <raylib.h>

/*
 * Estrutura com os recursos do jogo.
 * - spritesheet_atlas: atlas 32x32 (assets/tileset.png)
 * - player_sheet: folha do player (opcional, assets/player.png)
 * - rect_*: regiões do atlas para sprites específicos
 */
typedef struct Assets {
    Texture2D spritesheet_atlas;
    Texture2D player_sheet;      // opcional
<<<<<<< HEAD
    Texture2D map_background;
    Rectangle rect_platform;     // bloco sólido (ex.: (0,1) no atlas)
    Rectangle rect_trap_spike;   // trap/espinho (ex.: (0,6))
    Rectangle rect_goal_door;    // "porta"/goal (ex.: (0,5))
    Rectangle rect_button_play;  // botão "play" (placeholder, ex.: (0,4))
=======

    Rectangle rect_platform;     // bloco sólido
    Rectangle rect_trap_spike;   // trap/espinho
    Rectangle rect_trap_false;
    Rectangle rect_goal_door;    // "porta"/goal 
>>>>>>> 6e10015 (Tiles sendo desenhadas corretamente)
} Assets;

/* Inicializa/verifica assets e estrutura mínima de pastas.
 * Retorna false se algo crítico estiver faltando (ex.: tileset).
 */
bool assets_init(void);

/* Descarta recursos carregados (texturas, etc.). */
void assets_unload(void);

/* Acesso global aos assets carregados. */
Assets* GetAssets(void);

#endif // ASSETS_H
