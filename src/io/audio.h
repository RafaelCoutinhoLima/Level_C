#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

typedef enum {
    AUDIO_SFX_DIE = 0,
    AUDIO_SFX_GOAL = 1,
    AUDIO_SFX_JUMP = 2 
} AudioEvent;

bool audio_init(void);
void audio_update(void);
void audio_shutdown(void);

void audio_play_music(void); 
void audio_stop_music(void);
bool audio_is_music_playing(void);

void audio_play_event(AudioEvent e);

#endif