#include "particle.h"
#include <stdlib.h>
#include <math.h>

// Inicializa o sistema
void particle_system_init(ParticleSystem* ps) {
    if (!ps) return;
    ps->head = NULL;
    ps->count = 0;
    TraceLog(LOG_INFO, "[ParticleSystem] Inicializado");
}

// Limpa todas as partículas
void particle_system_clear(ParticleSystem* ps) {
    if (!ps) return;
    
    Particle* current = ps->head;
    while (current) {
        Particle* next = current->next;
        free(current);
        current = next;
    }
    
    ps->head = NULL;
    ps->count = 0;
    TraceLog(LOG_INFO, "[ParticleSystem] Limpo (%d partículas removidas)", ps->count);
}

// Adiciona uma partícula na lista
static void particle_add(ParticleSystem* ps, Particle p) {
    if (!ps) return;
    
    Particle* newParticle = (Particle*)malloc(sizeof(Particle));
    if (!newParticle) {
        TraceLog(LOG_ERROR, "[ParticleSystem] Falha ao alocar partícula");
        return;
    }
    
    *newParticle = p;
    newParticle->next = ps->head;  // ← Insere no início da lista
    ps->head = newParticle;
    ps->count++;
}

// Atualiza e remove partículas mortas
void particle_system_update(ParticleSystem* ps, float dt) {
    if (!ps) return;
    
    Particle** current = &ps->head;
    
    while (*current) {
        Particle* p = *current;
        
        // Atualiza física
        p->lifetime -= dt;
        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;
        p->velocity.y += 500.0f * dt;  // Gravidade
        
        // Remove se morreu
        if (p->lifetime <= 0.0f) {
            *current = p->next;  // ← Remove da lista encadeada
            free(p);
            ps->count--;
        } else {
            current = &p->next;
        }
    }
}

// Desenha todas as partículas
void particle_system_draw(const ParticleSystem* ps) {
    if (!ps) return;
    
    Particle* current = ps->head;
    
    while (current) {
        float alpha = current->lifetime / current->maxLifetime;
        Color c = current->color;
        c.a = (unsigned char)(alpha * 255);
        
        DrawCircleV(current->position, current->size, c);
        
        current = current->next;  // ← Percorre a lista
    }
}

// Emite explosão (quando player morre)
void particle_emit_explosion(ParticleSystem* ps, Vector2 position, int count) {
    if (!ps) return;
    
    for (int i = 0; i < count; i++) {
        float angle = ((float)GetRandomValue(0, 360)) * DEG2RAD;
        float speed = (float)GetRandomValue(50, 200);
        
        // Variação de cores ciano
        Color colors[] = {
            (Color){0, 255, 255, 255},    // Ciano puro
            (Color){0, 200, 255, 255},    // Ciano azulado
            (Color){100, 255, 255, 255},  // Ciano claro
            (Color){0, 180, 200, 255}     // Ciano escuro
        };
        Color particleColor = colors[GetRandomValue(0, 3)];
        
        Particle p = {
            .position = position,
            .velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed },
            .color = particleColor,
            .lifetime = (float)GetRandomValue(5, 15) / 10.0f,
            .maxLifetime = (float)GetRandomValue(5, 15) / 10.0f,
            .size = (float)GetRandomValue(2, 6),
            .next = NULL
        };
        
        particle_add(ps, p);
    }
    
    TraceLog(LOG_INFO, "[ParticleSystem] Explosão ciano emitida (%d partículas)", count);
}

// Emite poeira (quando player anda)
void particle_emit_dust(ParticleSystem* ps, Vector2 position) {
    if (!ps) return;
    
    for (int i = 0; i < 3; i++) {
        Particle p = {
            .position = position,
            .velocity = (Vector2){ 
                (float)GetRandomValue(-30, 30), 
                (float)GetRandomValue(-50, -20) 
            },
            .color = (Color){ 200, 200, 200, 255 },
            .lifetime = 0.3f,
            .maxLifetime = 0.3f,
            .size = 2.0f,
            .next = NULL
        };
        
        particle_add(ps, p);
    }
}

void particle_emit_goal(ParticleSystem* ps, Vector2 position) {
    if (!ps) return;
    
    // Explosão de estrelas douradas
    for (int i = 0; i < 40; i++) {
        float angle = ((float)GetRandomValue(0, 360)) * DEG2RAD;
        float speed = (float)GetRandomValue(150, 400);
        
        Color colors[] = { 
            MAGENTA, 
            ORANGE, 
            SKYBLUE,
            WHITE                         // Branco brilhante
        };
        Color c = colors[GetRandomValue(0, 3)];
        
        Particle p = {
            .position = position,
            .velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed - 100.0f },
            .color = c,
            .lifetime = (float)GetRandomValue(10, 20) / 10.0f,
            .maxLifetime = (float)GetRandomValue(10, 20) / 10.0f,
            .size = (float)GetRandomValue(4, 10),
            .next = NULL
        };
        
        particle_add(ps, p);
    }
    
    TraceLog(LOG_INFO, "[ParticleSystem] Goal ÉPICO emitido! (90 partículas)");
}