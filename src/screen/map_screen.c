// src/screen/map_screen.c
#include "map_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include "progress/progress.h"
#include "io/audio.h"
#include <raylib.h>

#define NODE_RADIUS 25.0f
#define FADE_SPEED  3.0f
#define ANIM_SPEED  4.0f

static LevelNode nodes[MAX_MAP_NODES];
static int  totalNodes        = 5;
static int  hoveredNodeIndex  = -1;

static float fadeAlpha        = 1.0f;
static bool  isFadingOut      = false;
static int   nextLevelId      = -1;

static Button btnBackToMenu;
static Button btnMusic;

static void SetupNodes(void) {
    nodes[0].levelId   = 1;
    nodes[0].position  = (Vector2){150, 300};
    nodes[0].state     = NODE_COMPLETED;
    nodes[0].animScale = 1.0f;

    nodes[1].levelId   = 2;
    nodes[1].position  = (Vector2){280, 200};
    nodes[1].state     = NODE_AVAILABLE;
    nodes[1].animScale = 1.0f;

    nodes[2].levelId   = 3;
    nodes[2].position  = (Vector2){410, 300};
    nodes[2].state     = NODE_LOCKED;
    nodes[2].animScale = 1.0f;

    nodes[3].levelId   = 4;
    nodes[3].position  = (Vector2){540, 200};
    nodes[3].state     = NODE_LOCKED;
    nodes[3].animScale = 1.0f;

    nodes[4].levelId   = 5;
    nodes[4].position  = (Vector2){670, 300};
    nodes[4].state     = NODE_LOCKED;
    nodes[4].animScale = 1.0f;
}

void map_screen_init(void) {
    TraceLog(LOG_INFO, "[Map] Init");
    audio_init();
    SetupNodes();

    fadeAlpha        = 1.0f;
    isFadingOut      = false;
    nextLevelId      = -1;
    hoveredNodeIndex = -1;

    btnBackToMenu = CreateButton(-1, 520, 200, 40, "Voltar ao Menu");
    btnMusic      = CreateButton(20, 20, 150, 30, audio_is_music_on() ? "Musica: ON" : "Musica: OFF");
}

void map_screen_update(float dt) {
    audio_update();

    Vector2 mousePos = GetMousePosition();

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
        if (CheckCollisionPointCircle(mousePos, nodes[i].position, NODE_RADIUS)) {
            hoveredNodeIndex = i;
            nodes[i].isHovered = true;
            if (nodes[i].animScale < 1.2f) nodes[i].animScale += ANIM_SPEED * dt;

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

    if (!isFadingOut) {
        if (UpdateButton(&btnBackToMenu)) {
            state_change(SCREEN_HOME);
        }

        if (UpdateButton(&btnMusic)) {
            audio_toggle_music();
            SetButtonText(&btnMusic, audio_is_music_on() ? "Musica: ON" : "Musica: OFF");
        }
    }
}

void map_screen_draw(void) {
    ClearBackground((Color){20, 30, 20, 255});

    for (int i = 0; i < totalNodes - 1; i++) {
        DrawLineEx(nodes[i].position, nodes[i+1].position, 4.0f, DARKGRAY);
    }

    for (int i = 0; i < totalNodes; i++) {
        Color coreColor;
        Color ringColor = WHITE;
        switch (nodes[i].state) {
            case NODE_LOCKED:    coreColor = (Color){80, 80, 80, 255}; ringColor = GRAY; break;
            case NODE_AVAILABLE: coreColor = (Color){0, 121, 241, 255}; break;
            case NODE_COMPLETED: coreColor = (Color){253, 249, 0, 255}; break;
        }

        float finalRadius = NODE_RADIUS * nodes[i].animScale;
        DrawCircleV(nodes[i].position, finalRadius + 3, ringColor);
        DrawCircleV(nodes[i].position, finalRadius, coreColor);

        DrawText(TextFormat("%d", nodes[i].levelId),
                 (int)nodes[i].position.x - 5,
                 (int)nodes[i].position.y - 10,
                 20, BLACK);
    }

    if (hoveredNodeIndex != -1 && nodes[hoveredNodeIndex].state != NODE_LOCKED) {
        LevelNode *n = &nodes[hoveredNodeIndex];
        int tx = (int)n->position.x + 20;
        int ty = (int)n->position.y - 60;
        DrawRectangle(tx, ty, 220, 50, Fade(BLACK, 0.9f));
        DrawRectangleLines(tx, ty, 220, 50, WHITE);
    } else if (hoveredNodeIndex != -1 && nodes[hoveredNodeIndex].state == NODE_LOCKED) {
        int tx = (int)nodes[hoveredNodeIndex].position.x + 20;
        int ty = (int)nodes[hoveredNodeIndex].position.y - 30;
        DrawRectangle(tx, ty, 100, 25, Fade(RED, 0.8f));
        DrawText("BLOQUEADO", tx + 10, ty + 5, 10, WHITE);
    }

    DrawText("MAPA DO MUNDO", (GetScreenWidth() - MeasureText("MAPA DO MUNDO", 40))/2, 50, 40, WHITE);
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
