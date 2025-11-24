// src/io/audio.c
#include "audio.h"
#include <stddef.h>
#include <raylib.h>

// -------------------- Volumes (0.0f a 1.0f) --------------------
#define MASTER_VOL 1.0f
#define MUSIC_VOL  1.0f
#define SFX_VOL    1.0f
// ---------------------------------------------------------------

typedef struct {
    bool initialized;
    bool musicOn;

    Music bg;
    Sound sfx_die;
    Sound sfx_goal;

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

    SetMasterVolume(MASTER_VOL);

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
        SetMusicVolume(G.bg, MUSIC_VOL);
        PlayMusicStream(G.bg);
        G.musicOn = true;
        TraceLog(LOG_INFO, "[audio] Música iniciada (ON, vol=%.2f)", (float)MUSIC_VOL);
    } else {
        G.musicOn = false;
    }

    if (G.dieLoaded)  SetSoundVolume(G.sfx_die,  SFX_VOL);
    if (G.goalLoaded) SetSoundVolume(G.sfx_goal, SFX_VOL);

    G.initialized = true;
    TraceLog(LOG_INFO, "[audio] init ok (master=%.2f, sfx=%.2f, music=%.2f, musicOn=%d)",
             (float)MASTER_VOL, (float)SFX_VOL, (float)MUSIC_VOL, G.musicOn);
    return true;
}

void audio_update(void) {
    if (!G.initialized) return;
    if (G.musicOn && G.bgLoaded) UpdateMusicStream(G.bg);
}

void audio_play_event(AudioEvent e) {
    if (!G.initialized) return;

    switch (e) {
        case AUDIO_SFX_DIE:
            if (G.dieLoaded) {
                SetSoundVolume(G.sfx_die, SFX_VOL);
                PlaySound(G.sfx_die);
                TraceLog(LOG_INFO, "[audio] play DIE (vol=%.2f)", (float)SFX_VOL);
            } else {
                TraceLog(LOG_WARNING, "[audio] DIE não disponível");
            }
            break;

        case AUDIO_SFX_GOAL:
            if (G.goalLoaded) {
                SetSoundVolume(G.sfx_goal, SFX_VOL);
                PlaySound(G.sfx_goal);
                TraceLog(LOG_INFO, "[audio] play GOAL (vol=%.2f)", (float)SFX_VOL);
            } else {
                TraceLog(LOG_WARNING, "[audio] GOAL não disponível");
            }
            break;

        default: break;
    }
}

void audio_toggle_music(void) {
    if (!G.initialized || !G.bgLoaded) return;

    if (G.musicOn) {
        PauseMusicStream(G.bg);
        G.musicOn = false;
        TraceLog(LOG_INFO, "[audio] Música pausada (OFF)");
    } else {
        SetMusicVolume(G.bg, MUSIC_VOL); // re-aplica volume ao retomar
        ResumeMusicStream(G.bg);
        G.musicOn = true;
        TraceLog(LOG_INFO, "[audio] Música retomada (ON, vol=%.2f)", (float)MUSIC_VOL);
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
