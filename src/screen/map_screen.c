// src/screen/map_screen.c
#include "map_screen.h"
#include "core/state.h"
#include "ui/button.h"
#include "progress/progress.h"
#include "io/audio.h"
#include <raylib.h>
#include <stdio.h>  // Para sprintf/TextFormat

#define NODE_RADIUS 25.0f      // Tamanho da bolinha da fase
#define FADE_SPEED 3.0f        // Velocidade da transição (maior = mais rápido)
#define ANIM_SPEED 4.0f        // Velocidade do "pulso" do botão

// --- Variáveis Estáticas (Estado da Tela) ---
static LevelNode nodes[MAX_MAP_NODES];
static int totalNodes = 5;     // Total de fases ativas
static int hoveredNodeIndex = -1; // Qual nó o mouse está em cima (-1 = nenhum)

// --- Controle de Transição (Fade)
static float fadeAlpha = 1.0f; // 1.0 = Tela preta, 0.0 = Tela visível
static bool isFadingOut = false;
static int nextLevelId = -1;   // Para onde vamos?

// --- Interface UI Fixa ---
static Button btnBackToMenu;
static Button btnMusic;

static void SetupNodes(void) {
    // Você pode usar uma lógica aqui para ler do progress.h qual fase está liberada
    // Por exemplo: int maxUnlocked = progress_get_max_unlocked();

    // --- FASE 1 ---
    nodes[0].levelId = 1;
    nodes[0].position = (Vector2){150, 300}; // Esquerda
    nodes[0].state = NODE_COMPLETED;         // Exemplo: Já passou
    nodes[0].animScale = 1.0f;

    // --- FASE 2 ---
    nodes[1].levelId = 2;
    nodes[1].position = (Vector2){280, 200}; // Sobe um pouco
    nodes[1].state = NODE_AVAILABLE;         // Exemplo: Disponível
    nodes[1].animScale = 1.0f;

    // --- FASE 3 ---
    nodes[2].levelId = 3;
    nodes[2].position = (Vector2){410, 300}; // Desce
    nodes[2].state = NODE_LOCKED;            
    nodes[2].animScale = 1.0f;
    // --- FASE 4 ---
    nodes[3].levelId = 4;
    nodes[3].position = (Vector2){540, 200}; // Sobe
    nodes[3].state = NODE_LOCKED;
    nodes[3].animScale = 1.0f;
    
    // --- FASE 5 ---
    nodes[4].levelId = 5;
    nodes[4].position = (Vector2){670, 300}; // Desce (Final)
    nodes[4].state = NODE_LOCKED;
    nodes[4].animScale = 1.0f;
    }
void map_screen_init(void){
    TraceLog(LOG_INFO, "[Map] Init");
    audio_init();
    SetupNodes();

    // Reseta variaveis de transição
    fadeAlpha = 1.0f;      // Começa preto para fazer Fade-In
    isFadingOut = false;
    nextLevelId = -1;
    hoveredNodeIndex = -1;

    // Botões fixos da UI
    btnBackToMenu = CreateButton(-1, 520, 200, 40, "Voltar ao Menu");
    btnMusic = CreateButton(20, 20, 150, 30, audio_is_music_on() ? "Musica: ON" : "Musica: OFF");
}
void map_screen_update(float dt){
    audio_update();
    Vector2 mousePos = GetMousePosition();

    //Lógica de Transição (Fade) 
    if (!isFadingOut) {
        // Fade In (Tela clareando ao entrar)
        if (fadeAlpha > 0.0f) {
            fadeAlpha -= FADE_SPEED * dt;
            if (fadeAlpha < 0.0f) fadeAlpha = 0.0f;
        }
    } else {
        // Fade Out (Tela escurecendo ao sair)
        fadeAlpha += FADE_SPEED * dt;
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            //mudança da tela dos niveis
            if (nextLevelId != -1) {
                progress_set_current_level(nextLevelId);
                state_change(SCREEN_PLAY);
            }
            return;
        }
    }

    // Se a tela estiver muito escura (transição), bloqueia cliques
    if (fadeAlpha > 0.1f && isFadingOut) return;

    hoveredNodeIndex = -1;

    for (int i = 0; i < totalNodes; i++) {
        // Verifica colisão Mouse vs Bolinha
        if (CheckCollisionPointCircle(mousePos, nodes[i].position, NODE_RADIUS)) {
            hoveredNodeIndex = i;
            nodes[i].isHovered = true;
            if (nodes[i].animScale < 1.2f) nodes[i].animScale += ANIM_SPEED * dt;
            // Clique para entrar na fase
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (nodes[i].state != NODE_LOCKED) {
                    nextLevelId = nodes[i].levelId;
                    isFadingOut = true; // Começa a escurecer a tela
                    //PlaySound(sndSelect);
                } else {
                    //PlaySound(sndLocked);
                }
            }
        } else {
            nodes[i].isHovered = false;
            if (nodes[i].animScale > 1.0f) nodes[i].animScale -= ANIM_SPEED * dt;
        }
    }
    // Só permite clicar nos botões se não estiver saindo da tela
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
void map_screen_draw(void){
    ClearBackground((Color){20, 30, 20, 255});
    // 1. Desenha as linhas conectando as fases
    for (int i = 0; i < totalNodes - 1; i++) {
        DrawLineEx(nodes[i].position, nodes[i+1].position, 4.0f, DARKGRAY);
    }
    // 2. Desenha os Nós
    for (int i = 0; i < totalNodes; i++) {
        Color coreColor;
        Color ringColor = WHITE;
        // Define cor baseada no estado
        switch(nodes[i].state) {
            case NODE_LOCKED:    coreColor = (Color){80, 80, 80, 255}; ringColor = GRAY; break; // Cinza
            case NODE_AVAILABLE: coreColor = (Color){0, 121, 241, 255}; break; // Azul
            case NODE_COMPLETED: coreColor = (Color){253, 249, 0, 255}; break; // Dourado
        }
        // Desenha
        float finalRadius = NODE_RADIUS * nodes[i].animScale;
        DrawCircleV(nodes[i].position, finalRadius + 3, ringColor); // Borda
        DrawCircleV(nodes[i].position, finalRadius, coreColor);     // Miolo
        // Número da fase
        DrawText(TextFormat("%d", nodes[i].levelId), 
                 (int)nodes[i].position.x - 5, 
                 (int)nodes[i].position.y - 10, 
                 20, BLACK);
    }
    // Desenha DEPOIS dos nós para ficar por cima de tudo
    if (hoveredNodeIndex != -1 && nodes[hoveredNodeIndex].state != NODE_LOCKED) {
        LevelNode *n = &nodes[hoveredNodeIndex];
        
        // Posição do tooltip um pouco acima do nó
        int tx = (int)n->position.x + 20;
        int ty = (int)n->position.y - 60;
        // Fundo preto semitransparente
        DrawRectangle(tx, ty, 220, 50, Fade(BLACK, 0.9f));
        DrawRectangleLines(tx, ty, 220, 50, WHITE);
       }
    else if (hoveredNodeIndex != -1 && nodes[hoveredNodeIndex].state == NODE_LOCKED) {
        // Tooltip simples para fase bloqueada
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

void map_screen_unload(void){
    TraceLog(LOG_INFO, "[Map] unload");
}

GameState map_screen_state(void){
    return (GameState){
        .init = map_screen_init,
        .update = map_screen_update,
        .draw = map_screen_draw,
        .unload = map_screen_unload,
    };
}