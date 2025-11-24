// src/io/audio.c
#include "audio.h"
#include <stddef.h>
#include <raylib.h>

// Estado interno do módulo de áudio
typedef struct {
    bool initialized;
    bool musicOn;

    Music bg;        // música de fundo (stream)
    Sound sfx_die;   // efeito: morte
    Sound sfx_goal;  // efeito: goal

    bool bgLoaded;
    bool dieLoaded;
    bool goalLoaded;
} AudioState;

static AudioState G = {0};

bool audio_init(void) {
    if (G.initialized) return true;

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "[audio] Dispositivo de áudio não pronto");
        return false;
    }

    // Volume global
    SetMasterVolume(1.0f);

    // Carrega trilha e efeitos
    G.bg        = LoadMusicStream("assets/audio/bg_main.ogg");
    G.bgLoaded  = (G.bg.stream.buffer != NULL);
    TraceLog(G.bgLoaded ? LOG_INFO : LOG_WARNING,
             G.bgLoaded ? "[audio] bg_main.ogg OK" : "[audio] bg_main.ogg não encontrado (sem música)");

    G.sfx_die   = LoadSound("assets/audio/die.wav");
    G.dieLoaded = (G.sfx_die.frameCount > 0);
    TraceLog(G.dieLoaded ? LOG_INFO : LOG_WARNING,
             G.dieLoaded ? "[audio] die.wav OK" : "[audio] die.wav não encontrado");

    G.sfx_goal  = LoadSound("assets/audio/goal.wav");
    G.goalLoaded= (G.sfx_goal.frameCount > 0);
    TraceLog(G.goalLoaded ? LOG_INFO : LOG_WARNING,
             G.goalLoaded ? "[audio] goal.wav OK" : "[audio] goal.wav não encontrado");

    if (G.bgLoaded) {
        SetMusicVolume(G.bg, 0.8f);
        PlayMusicStream(G.bg);
        G.musicOn = true;
        TraceLog(LOG_INFO, "[audio] Música iniciada (ON)");
    } else {
        G.musicOn = false;
    }

    G.initialized = true;
    TraceLog(LOG_INFO, "[audio] init ok (musicOn=%d)", G.musicOn);
    return true;
}

void audio_update(void) {
    if (!G.initialized) return;
    if (G.musicOn && G.bgLoaded) {
        UpdateMusicStream(G.bg);
    }
}

void audio_play_event(AudioEvent e) {
    if (!G.initialized) return;

    switch (e) {
        case AUDIO_SFX_DIE:
            if (G.dieLoaded) {
                SetSoundVolume(G.sfx_die, 1.0f);
                PlaySound(G.sfx_die);
                TraceLog(LOG_INFO, "[audio] play DIE");
            } else {
                TraceLog(LOG_WARNING, "[audio] DIE não disponível");
            }
            break;

        case AUDIO_SFX_GOAL:
            if (G.goalLoaded) {
                SetSoundVolume(G.sfx_goal, 1.0f);
                PlaySound(G.sfx_goal);
                TraceLog(LOG_INFO, "[audio] play GOAL");
            } else {
                TraceLog(LOG_WARNING, "[audio] GOAL não disponível");
            }
            break;

        default:
            break;
    }
}

void audio_toggle_music(void) {
    if (!G.initialized || !G.bgLoaded) return;

    if (G.musicOn) {
        PauseMusicStream(G.bg);
        G.musicOn = false;
        TraceLog(LOG_INFO, "[audio] Música pausada (OFF)");
    } else {
        ResumeMusicStream(G.bg);
        G.musicOn = true;
        TraceLog(LOG_INFO, "[audio] Música retomada (ON)");
    }
}

bool audio_is_music_on(void) {
    return (G.initialized && G.musicOn);
}

void audio_shutdown(void) {
    if (!G.initialized) return;

    if (G.bgLoaded) {
        StopMusicStream(G.bg);
        UnloadMusicStream(G.bg);
    }
    if (G.dieLoaded)  UnloadSound(G.sfx_die);
    if (G.goalLoaded) UnloadSound(G.sfx_goal);

    CloseAudioDevice();
    G = (AudioState){0};
    TraceLog(LOG_INFO, "[audio] shutdown");
}
