#include "map_screen.h"
#include "core/state.h"
#include "progress/progress.h"
#include "io/audio.h"
#include "io/assets.h" 
#include <raylib.h>
#include <math.h> 

#define GRAVE_W 117.0f
#define GRAVE_H 115.0f

#define FADE_SPEED  3.0f
#define ANIM_SPEED  6.0f 

static LevelNode nodes[MAX_MAP_NODES];
static int  totalNodes        = 10;
static int  hoveredNodeIndex  = -1;

static float fadeAlpha        = 1.0f;
static bool  isFadingOut      = false;
static int   nextLevelId      = -1;
static float pulseTimer       = 0.0f; 

static Rectangle btnBackArea;
static bool btnBackHovered = false;
static float btnAnimScale = 1.0f; 

static Texture2D lockerTex; 

static void SetupNodes(void) {
    
    // FILEIRA 1
    nodes[0] = (LevelNode){1, {213, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[1] = (LevelNode){2, {456, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[2] = (LevelNode){3, {673, 269}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[3] = (LevelNode){4, {888, 269}, NODE_AVAILABLE, false, 1.0f}; 

    // FILEIRA 2
    nodes[4] = (LevelNode){5, {906, 395}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[5] = (LevelNode){6, {689, 395}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[6] = (LevelNode){7, {459, 395}, NODE_AVAILABLE, false, 1.0f}; 

    // FILEIRA 3
    nodes[7] = (LevelNode){8, {212, 449}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[8] = (LevelNode){9, {346, 530}, NODE_AVAILABLE, false, 1.0f}; 
    nodes[9] = (LevelNode){10,{572, 530}, NODE_AVAILABLE, false, 1.0f}; 

    int maxUnlocked = progress_get_max_unlocked();
    for(int i=0; i < totalNodes; i++){
        if (nodes[i].levelId > maxUnlocked) {
            nodes[i].state = NODE_LOCKED;
        } else {
            nodes[i].state = NODE_AVAILABLE;
        }
    }
}

void map_screen_init(void) {
    TraceLog(LOG_INFO, "[Map] Init");
    audio_init();
    SetupNodes();

    lockerTex = LoadTexture("assets/locker.png"); 
    SetTextureFilter(lockerTex, TEXTURE_FILTER_POINT);

    fadeAlpha        = 1.0f;
    isFadingOut      = false;
    nextLevelId      = -1;
    hoveredNodeIndex = -1;
    pulseTimer       = 0.0f;
    
    btnBackArea = (Rectangle){ 1045.0f, 623.0f, 211.0f, 73.0f };
    btnBackHovered = false;
    btnAnimScale = 1.0f;
}

void map_screen_update(float dt) {
    audio_update();
    Vector2 mousePos = GetMousePosition();
    pulseTimer += dt * 5.0f; 

    if (!isFadingOut) {
        btnBackHovered = CheckCollisionPointRec(mousePos, btnBackArea);
        if (btnBackHovered) {
            if (btnAnimScale < 1.1f) btnAnimScale += ANIM_SPEED * dt;
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state_change(SCREEN_HOME);
            }
        } else {
            if (btnAnimScale > 1.0f) btnAnimScale -= ANIM_SPEED * dt;
        }
    }

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
        Rectangle nodeRect = {
            nodes[i].position.x, 
            nodes[i].position.y, 
            GRAVE_W, 
            GRAVE_H
        };

        if (CheckCollisionPointRec(mousePos, nodeRect)) {
            hoveredNodeIndex = i;
            nodes[i].isHovered = true;
            if (nodes[i].animScale < 1.1f) nodes[i].animScale += ANIM_SPEED * dt;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (nodes[i].state != NODE_LOCKED) {
                    nextLevelId = nodes[i].levelId;
                    isFadingOut = true;
                }
            }
        } else {
            nodes[i].isHovered = false;
            if (nodes[i].animScale > 1.0f) nodes[i].animScale -= ANIM_SPEED * dt;
        }
    }
}

void map_screen_draw(void) {
    ClearBackground(BLACK);

    Texture2D bg = GetAssets()->map_background;
    if (bg.id != 0) {
        Rectangle source = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
        Rectangle dest   = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(bg, source, dest, (Vector2){0,0}, 0.0f, WHITE);
        
        if (btnAnimScale > 1.001f) {
            float scaleX = (float)bg.width / GetScreenWidth();
            float scaleY = (float)bg.height / GetScreenHeight();

            Rectangle sourceRec = {
                btnBackArea.x * scaleX,
                btnBackArea.y * scaleY,
                btnBackArea.width * scaleX,
                btnBackArea.height * scaleY
            };

            float newW = btnBackArea.width * btnAnimScale;
            float newH = btnBackArea.height * btnAnimScale;
            
            Rectangle destRec = {
                btnBackArea.x - (newW - btnBackArea.width) / 2.0f,
                btnBackArea.y - (newH - btnBackArea.height) / 2.0f,
                newW,
                newH
            };
            DrawTexturePro(bg, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);
        }
    }

    for (int i = 0; i < totalNodes; i++) {
        float currentScale = nodes[i].animScale;
        float drawW = GRAVE_W;
        float drawH = GRAVE_H;
        float drawX = nodes[i].position.x;
        float drawY = nodes[i].position.y;

        if (nodes[i].isHovered && bg.id != 0) {
            float scaleX = (float)bg.width / GetScreenWidth();
            float scaleY = (float)bg.height / GetScreenHeight();

            Rectangle sourceRec = {
                nodes[i].position.x * scaleX,
                nodes[i].position.y * scaleY,
                GRAVE_W * scaleX,
                GRAVE_H * scaleY
            };

            drawW = GRAVE_W * currentScale;
            drawH = GRAVE_H * currentScale;
            
            drawX = nodes[i].position.x - (drawW - GRAVE_W) / 2.0f;
            drawY = nodes[i].position.y - (drawH - GRAVE_H) / 2.0f - (currentScale > 1.0f ? 5.0f : 0.0f);

            Rectangle destRec = { drawX, drawY, drawW, drawH };

            Color tint = (nodes[i].state == NODE_LOCKED) ? LIGHTGRAY : WHITE;
            DrawTexturePro(bg, sourceRec, destRec, (Vector2){0,0}, 0.0f, tint);
        }

        if (nodes[i].state == NODE_LOCKED) {
            // Define o tamanho alvo: 50% da largura da área da lápide
            float targetLockWidth = drawW * 0.5f; 
            
            // Calcula a escala para a imagem ficar desse tamanho
            float lockScale = targetLockWidth / (float)lockerTex.width;
            
            float lockW = lockerTex.width * lockScale;
            float lockH = lockerTex.height * lockScale;

            float lockX = drawX + (drawW - lockW) / 2.0f;
            float lockY = drawY + (drawH - lockH) / 2.0f;


            DrawTextureEx(lockerTex, (Vector2){lockX + 2, lockY + 2}, 0.0f, lockScale, (Color){0,0,0,100});
            DrawTextureEx(lockerTex, (Vector2){lockX, lockY}, 0.0f, lockScale, WHITE);
        }
    }
    
    // SCORE DE MORTES 
    int deaths = progress_get_total_deaths();
    DrawText(TextFormat("%d", deaths), 105, 638, 55, WHITE);

    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
    }
}

void map_screen_unload(void) {
    UnloadTexture(lockerTex);
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