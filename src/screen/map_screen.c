// src/screen/map_screen.c
#include "map_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include "progress/progress.h"
#include "io/audio.h"
#include <raylib.h>
#include <stdio.h>

// --- Configs visuais do mapa de fases (se você já tinha outra versão, mantenha sua lógica) ---
#define NODE_RADIUS 25.0f
#define FADE_SPEED  3.0f
#define ANIM_SPEED  4.0f

// Se você já possui LevelNode/estados em outro header, use-os daqui.
// Aqui vai um stub simples para manter o arquivo autossuficiente:
typedef enum { NODE_LOCKED, NODE_AVAILABLE, NODE_COMPLETED } NodeState;
typedef struct {
    int levelId;
    Vector2 position;
    NodeState state;
    float animScale;
    bool isHovered;
} LevelNode;

#define MAX_MAP_NODES 5

static LevelNode nodes[MAX_MAP_NODES];
static int  totalNodes = 5;
static int  hoveredNodeIndex = -1;

// --- Fade/Transição ---
static float fadeAlpha = 1.0f;
static bool  isFadingOut = false;
static int   nextLevelId = -1;

// --- Botões ---
static Button btnBackToMenu;
static Button btnMusic;

// --- Helpers ---
static void SetupNodes(void) {
    nodes[0] = (LevelNode){ .levelId=1, .position={150,300}, .state=NODE_COMPLETED, .animScale=1.0f };
    nodes[1] = (LevelNode){ .levelId=2, .position={280,200}, .state=NODE_AVAILABLE, .animScale=1.0f };
    nodes[2] = (LevelNode){ .levelId=3, .position={410,300}, .state=NODE_LOCKED,    .animScale=1.0f };
    nodes[3] = (LevelNode){ .levelId=4, .position={540,200}, .state=NODE_LOCKED,    .animScale=1.0f };
    nodes[4] = (LevelNode){ .levelId=5, .position={670,300}, .state=NODE_LOCKED,    .animScale=1.0f };
}

static void update_music_button_label(void) {
    SetButtonText(&btnMusic, audio_is_music_on() ? "Música: ON (M)" : "Música: OFF (M)");
}

// --- Ciclo de vida da tela ---
void map_screen_init(void){
    TraceLog(LOG_INFO, "[Map] Init");

    // Garante áudio inicializado (idempotente)
    audio_init();

    SetupNodes();

    fadeAlpha     = 1.0f;
    isFadingOut   = false;
    nextLevelId   = -1;
    hoveredNodeIndex = -1;

    btnBackToMenu = CreateButton(-1, 520, 200, 40, "Voltar ao Menu");
    btnMusic      = CreateButton(20,  20, 150, 30, "Música: ...");
    update_music_button_label(); // rótulo conforme estado atual
}

void map_screen_update(float dt){
    audio_update();

    // Fade
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

    // Atalho de teclado para alternar música (M)
    if (IsKeyPressed(KEY_M)) {
        audio_toggle_music();
        update_music_button_label();
    }

    // Se estiver no fade-out, bloqueia interação
    if (fadeAlpha > 0.1f && isFadingOut) return;

    // Hover/check de nós
    Vector2 mousePos = GetMousePosition();
    hoveredNodeIndex = -1;

    for (int i = 0; i < totalNodes; i++) {
        bool hover = CheckCollisionPointCircle(mousePos, nodes[i].position, NODE_RADIUS);
        nodes[i].isHovered = hover;
        if (hover) {
            hoveredNodeIndex = i;
            if (nodes[i].animScale < 1.2f) nodes[i].animScale += ANIM_SPEED * dt;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (nodes[i].state != NODE_LOCKED) {
                    nextLevelId = nodes[i].levelId;
                    isFadingOut = true;
                }
            }
        } else {
            if (nodes[i].animScale > 1.0f) nodes[i].animScale -= ANIM_SPEED * dt;
        }
    }

    // Botões (só se não estiver saindo)
    if (!isFadingOut) {
        if (UpdateButton(&btnBackToMenu)) {
            state_change(SCREEN_HOME);
        }

        if (UpdateButton(&btnMusic)) {
            audio_toggle_music();
            update_music_button_label();
        }
    }
}

void map_screen_draw(void){
    ClearBackground((Color){20, 30, 20, 255});

    // Conexões
    for (int i = 0; i < totalNodes - 1; i++) {
        DrawLineEx(nodes[i].position, nodes[i+1].position, 4.0f, DARKGRAY);
    }

    // Nós
    for (int i = 0; i < totalNodes; i++) {
        Color coreColor, ringColor = WHITE;
        switch(nodes[i].state) {
            case NODE_LOCKED:    coreColor = (Color){80, 80, 80, 255}; ringColor = GRAY; break;
            case NODE_AVAILABLE: coreColor = (Color){0, 121, 241, 255}; break;
            case NODE_COMPLETED: coreColor = (Color){253, 249, 0, 255}; break;
        }
        float r = NODE_RADIUS * nodes[i].animScale;
        DrawCircleV(nodes[i].position, r + 3, ringColor);
        DrawCircleV(nodes[i].position, r, coreColor);

        DrawText(TextFormat("%d", nodes[i].levelId),
                 (int)nodes[i].position.x - 5,
                 (int)nodes[i].position.y - 10,
                 20, BLACK);
    }

    // Tooltip simples
    if (hoveredNodeIndex != -1) {
        int tx = (int)nodes[hoveredNodeIndex].position.x + 20;
        int ty = (int)nodes[hoveredNodeIndex].position.y - 30;
        Color tip = (nodes[hoveredNodeIndex].state == NODE_LOCKED) ? RED : BLACK;
        DrawRectangle(tx, ty, 140, 25, Fade(tip, 0.8f));
        DrawText((nodes[hoveredNodeIndex].state == NODE_LOCKED) ? "BLOQUEADO" : "ENTRAR",
                 tx + 10, ty + 5, 12, WHITE);
    }

    DrawText("MAPA DO MUNDO",
             (GetScreenWidth() - MeasureText("MAPA DO MUNDO", 40))/2, 50, 40, WHITE);

    DrawButton(btnBackToMenu);
    DrawButton(btnMusic);

    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeAlpha));
    }
}

void map_screen_unload(void){
    TraceLog(LOG_INFO, "[Map] unload");
}

GameState map_screen_state(void){
    return (GameState){
        .init   = map_screen_init,
        .update = map_screen_update,
        .draw   = map_screen_draw,
        .unload = map_screen_unload,
    };
}
