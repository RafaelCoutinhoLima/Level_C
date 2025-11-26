#pragma once

#include <raylib.h>
#include <stdbool.h>

typedef struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifetime;      // Tempo de vida restante
    float maxLifetime;   // Tempo de vida total
    float size;
    struct Particle* next;  // ← LISTA ENCADEADA
} Particle;

typedef struct ParticleSystem {
    Particle* head;  // ← Primeiro nó da lista
    int count;
} ParticleSystem;

// Funções
void particle_system_init(ParticleSystem* ps);
void particle_system_clear(ParticleSystem* ps);
void particle_system_update(ParticleSystem* ps, float dt);
void particle_system_draw(const ParticleSystem* ps);

// Emissores de partículas
void particle_emit_explosion(ParticleSystem* ps, Vector2 position, int count);
void particle_emit_dust(ParticleSystem* ps, Vector2 position);
void particle_emit_goal(ParticleSystem* ps, Vector2 position);