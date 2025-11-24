#include "map_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include "progress/progress.h"
#include "io/audio.h"
#include "io/assets.h" 
#include <raylib.h>
#include <math.h> 

//DIMENSÕES CALIBRADAS
#define GRAVE_W 117.0f
#define GRAVE_H 115.0f

#define FADE_SPEED  3.0f
#define ANIM_SPEED  6.0f // Velocidade do pulo

static LevelNode nodes[MAX_MAP_NODES];
static int  totalNodes        = 10;
static int  hoveredNodeIndex  = -1;

static float fadeAlpha        = 1.0f;
static bool  isFadingOut      = false;
static int   nextLevelId      = -1;
static float pulseTimer       = 0.0f; 

static Button btnBackToMenu;
static Button btnMusic;

static void SetupNodes(void) {
    //COORDENADAS EXATAS (TOP-LEFT)

    // FILEIRA 1 (Topo - OK)
    nodes[0] = (LevelNode){1, {213, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[1] = (LevelNode){2, {456, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[2] = (LevelNode){3, {673, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[3] = (LevelNode){4, {888, 269}, NODE_AVAILABLE, false, 1.0f}; 

    // FILEIRA 2 (Meio - OK)
    nodes[4] = (LevelNode){5, {906, 395}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[5] = (LevelNode){6, {689, 395}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[6] = (LevelNode){7, {459, 395}, NODE_AVAILABLE, false, 1.0f}; 

    // FILEIRA 3 (Baixo)
    nodes[7] = (LevelNode){8, {212, 449}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[8] = (LevelNode){9, {346, 530}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[9] = (LevelNode){10,{572, 530}, NODE_AVAILABLE, false, 1.0f}; 

    // MODO TESTE (Tudo liberado)
    for(int i=0; i < totalNodes; i++){
        nodes[i].state = NODE_AVAILABLE; 
    }
}

void map_screen_init(void) {
    TraceLog(LOG_INFO, "[Map] Init");
    audio_init();
    SetupNodes();

    fadeAlpha        = 1.0f;
    isFadingOut      = false;
    nextLevelId      = -1;
    hoveredNodeIndex = -1;
    pulseTimer       = 0.0f;

    btnBackToMenu = CreateButton(GetScreenWidth() - 220, GetScreenHeight() - 60, 200, 40, "Voltar");
    btnMusic      = CreateButton(20, 20, 150, 30, audio_is_music_on() ? "Musica: ON" : "Musica: OFF");
}

void map_screen_update(float dt) {
    audio_update();
    Vector2 mousePos = GetMousePosition();
    pulseTimer += dt * 5.0f; 

    if (!isFadingOut) {
        if (fadeAlpha > 0.0f) {
            fadeAlpha -= FADE_SPEED * dt;
            if (fadeAlpha < 0.0f) fadeAlpha = 0.0f;
        }
    } else {
        fadeAlpha += FADE_SPEED * dt;
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            if (nextLevelId != -1) {
                progress_set_current_level(nextLevelId);
                state_change(SCREEN_PLAY);
            }
            return;
        }
    }

    if (fadeAlpha > 0.1f && isFadingOut) return;

    hoveredNodeIndex = -1;
    for (int i = 0; i < totalNodes; i++) {
        // Colisão retangular precisa
        Rectangle nodeRect = {
            nodes[i].position.x, 
            nodes[i].position.y, 
            GRAVE_W, 
            GRAVE_H
        };

        if (CheckCollisionPointRec(mousePos, nodeRect)) {
            hoveredNodeIndex = i;
            nodes[i].isHovered = true;
            
            // Animação de Zoom
            if (nodes[i].animScale < 1.1f) nodes[i].animScale += ANIM_SPEED * dt;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (nodes[i].state != NODE_LOCKED) {
                    nextLevelId = nodes[i].levelId;
                    isFadingOut = true;
                }
            }
        } else {
            nodes[i].isHovered = false;
            // Volta ao normal
            if (nodes[i].animScale > 1.0f) nodes[i].animScale -= ANIM_SPEED * dt;
        }
    }

    if (!isFadingOut) {
        if (UpdateButton(&btnBackToMenu)) state_change(SCREEN_HOME);
        if (UpdateButton(&btnMusic)) {
            audio_toggle_music();
            SetButtonText(&btnMusic, audio_is_music_on() ? "Musica: ON" : "Musica: OFF");
        }
    }
}

void map_screen_draw(void) {
    ClearBackground(BLACK);

    Texture2D bg = GetAssets()->map_background;
    
    //Desenha o fundo completo
    if (bg.id != 0) {
        Rectangle source = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
        Rectangle dest   = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(bg, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }

    //Efeito de PULO (Zoom)
    for (int i = 0; i < totalNodes; i++) {
        if (nodes[i].state == NODE_LOCKED) continue;

        if (nodes[i].isHovered && bg.id != 0) {
            float scale = nodes[i].animScale; 

            // Conversão de escala Tela -> Imagem Original
            float scaleX = (float)bg.width / GetScreenWidth();
            float scaleY = (float)bg.height / GetScreenHeight();

            Rectangle sourceRec = {
                nodes[i].position.x * scaleX,
                nodes[i].position.y * scaleY,
                GRAVE_W * scaleX,
                GRAVE_H * scaleY
            };

            float newW = GRAVE_W * scale;
            float newH = GRAVE_H * scale;
            
            Rectangle destRec = {
                nodes[i].position.x - (newW - GRAVE_W) / 2.0f, 
                nodes[i].position.y - (newH - GRAVE_H) / 2.0f - (scale > 1.0f ? 5.0f : 0.0f), 
                newW,
                newH
            };

            // Desenha o recorte "pulando"
            DrawTexturePro(bg, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);
            
        }
    }

    DrawButton(btnBackToMenu);
    DrawButton(btnMusic);

    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
    }
}

void map_screen_unload(void) {
    TraceLog(LOG_INFO, "[Map] unload");
}

GameState map_screen_state(void) {
    return (GameState){
        .init   = map_screen_init,
        .update = map_screen_update,
        .draw   = map_screen_draw,
        .unload = map_screen_unload,
    };
}