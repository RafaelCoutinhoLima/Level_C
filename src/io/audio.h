// src/io/audio.h
#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

// Eventos de SFX (use em audio_play_event)
typedef enum {
    AUDIO_SFX_DIE = 0,    // morte do player
    AUDIO_SFX_GOAL = 1    // chegou no goal
} AudioEvent;

// Inicializa dispositivo de áudio e carrega assets (música + sfx)
bool audio_init(void);

// Atualiza o streaming da música (chamar a cada frame)
void audio_update(void);

// Dispara SFX por evento sem expor IDs mágicos
void audio_play_event(AudioEvent e);

// Liga/pausa a música de fundo
void audio_toggle_music(void);

// Estado atual da música (true = tocando)
bool audio_is_music_on(void);

// Descarrega áudio e fecha dispositivo
void audio_shutdown(void);

#endif // AUDIO_H
