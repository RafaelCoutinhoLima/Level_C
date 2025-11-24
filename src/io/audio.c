// src/io/audio.c
#include "audio.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define MASTER_VOL 1.0f
#define MUSIC_VOL  1.0f
#define SFX_VOL    1.0f

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

// tenta "path" e "../path"
static bool file_exists_multitry(const char* rel) {
    if (FileExists(rel)) return true;
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return FileExists(up);
}
static Music load_music_multitry(const char* rel) {
    if (FileExists(rel)) return LoadMusicStream(rel);
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return LoadMusicStream(up);
}
static Sound load_sound_multitry(const char* rel) {
    if (FileExists(rel)) return LoadSound(rel);
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return LoadSound(up);
}

bool audio_init(void) {
    if (G.initialized) return true;

    const char* wd = GetWorkingDirectory();
    TraceLog(LOG_INFO, "[audio] working dir: %s", wd ? wd : "(null)");

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "[audio] Dispositivo de audio nao pronto");
        return false;
    }

    SetMasterVolume(MASTER_VOL);

    const char* PATH_MUSIC = "assets/audio/bg_main.ogg";
    const char* PATH_DIE   = "assets/audio/die.wav";
    const char* PATH_GOAL  = "assets/audio/goal.wav";

    // checagens de existência
    TraceLog(LOG_INFO, "[audio] exists bg?  %d", file_exists_multitry(PATH_MUSIC));
    TraceLog(LOG_INFO, "[audio] exists die? %d", file_exists_multitry(PATH_DIE));
    TraceLog(LOG_INFO, "[audio] exists goal?%d", file_exists_multitry(PATH_GOAL));

    // carrega
    G.bg = load_music_multitry(PATH_MUSIC);
    G.bgLoaded = (G.bg.stream.buffer != NULL);
    TraceLog(G.bgLoaded ? LOG_INFO : LOG_WARNING,
             G.bgLoaded ? "[audio] bg_main.ogg OK" : "[audio] bg_main.ogg NAO carregado");

    G.sfx_die = load_sound_multitry(PATH_DIE);
    G.dieLoaded = (G.sfx_die.frameCount > 0);
    TraceLog(G.dieLoaded ? LOG_INFO : LOG_WARNING,
             G.dieLoaded ? "[audio] die.wav OK" : "[audio] die.wav NAO carregado");

    G.sfx_goal = load_sound_multitry(PATH_GOAL);
    G.goalLoaded = (G.sfx_goal.frameCount > 0);
    TraceLog(G.goalLoaded ? LOG_INFO : LOG_WARNING,
             G.goalLoaded ? "[audio] goal.wav OK" : "[audio] goal.wav NAO carregado");

    // volumes
    if (G.bgLoaded) {
        SetMusicVolume(G.bg, MUSIC_VOL);
        PlayMusicStream(G.bg);
        G.musicOn = true;
        TraceLog(LOG_INFO, "[audio] musica ON (vol=%.2f)", (float)MUSIC_VOL);
    } else {
        G.musicOn = false;
        TraceLog(LOG_INFO, "[audio] sem musica (seguindo)");
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
            if (G.dieLoaded) { SetSoundVolume(G.sfx_die, SFX_VOL); PlaySound(G.sfx_die); }
            else TraceLog(LOG_WARNING, "[audio] DIE indisponivel");
            break;
        case AUDIO_SFX_GOAL:
            if (G.goalLoaded) { SetSoundVolume(G.sfx_goal, SFX_VOL); PlaySound(G.sfx_goal); }
            else TraceLog(LOG_WARNING, "[audio] GOAL indisponivel");
            break;
        default: break;
    }
}

void audio_toggle_music(void) {
    if (!G.initialized || !G.bgLoaded) { TraceLog(LOG_INFO, "[audio] toggle sem musica"); return; }
    if (G.musicOn) { PauseMusicStream(G.bg); G.musicOn = false; TraceLog(LOG_INFO, "[audio] musica OFF"); }
    else           { SetMusicVolume(G.bg, MUSIC_VOL); ResumeMusicStream(G.bg); G.musicOn = true; TraceLog(LOG_INFO, "[audio] musica ON"); }
}

bool audio_is_music_on(void) { return (G.initialized && G.musicOn); }

void audio_shutdown(void) {
    if (!G.initialized) return;
    if (G.bgLoaded)   { StopMusicStream(G.bg); UnloadMusicStream(G.bg); }
    if (G.dieLoaded)  UnloadSound(G.sfx_die);
    if (G.goalLoaded) UnloadSound(G.sfx_goal);
    CloseAudioDevice();
    G = (AudioState){0};
    TraceLog(LOG_INFO, "[audio] shutdown");
}
