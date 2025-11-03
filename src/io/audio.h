#ifndef AUDIO_H
#define AUDIO_H
#include <stdbool.h>

bool audio_init(void);
void audio_update(void);
void audio_play_sfx(int id); // id simples por enquanto
void audio_shutdown(void);

#endif
