#ifndef ASSETS_H
#define ASSETS_H

#include <stdbool.h>
#include <raylib.h>

typedef struct Assets {
    Texture2D spritesheet_atlas;
    Texture2D player_sheet;      
    Texture2D map_background;
    Font gameFont;
    Rectangle rect_platform;     // bloco sólido
    Rectangle rect_trap_spike;   // trap/espinho
    Rectangle rect_trap_false;      //trap falsa
    Rectangle rect_goal_door;    // "porta"/goal 
    Rectangle rect_spike_false;     //spike falsa 
} Assets;

bool assets_init(void);

void assets_unload(void);

Assets* GetAssets(void);

#endif // ASSETS_H
