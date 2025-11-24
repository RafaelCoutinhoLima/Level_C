// src/io/audio.h
#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

typedef enum {
    AUDIO_SFX_DIE = 0,
    AUDIO_SFX_GOAL = 1
} AudioEvent;

bool audio_init(void);
void audio_update(void);
void audio_play_event(AudioEvent e);
void audio_toggle_music(void);
bool audio_is_music_on(void);
void audio_shutdown(void);

#endif // AUDIO_H
