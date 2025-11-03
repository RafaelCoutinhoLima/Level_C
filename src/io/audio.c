#include "audio.h"
#include <raylib.h>

bool audio_init(void) {
    TraceLog(LOG_INFO, "[audio] init (stub)");
    // InitAudioDevice(); // quando quiser ativar de verdade
    return true;
}

void audio_update(void) {
    // Atualizacoes futuras de audio
}

void audio_play_sfx(int id) {
    TraceLog(LOG_INFO, "[audio] play sfx id=%d (stub)", id);
}

void audio_shutdown(void) {
    TraceLog(LOG_INFO, "[audio] shutdown (stub)");
    // CloseAudioDevice();
}
