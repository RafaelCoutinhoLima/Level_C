#include "audio.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h> 

#define MASTER_VOL 1.0f
#define MUSIC_VOL  0.6f 
#define SFX_VOL    0.6f

typedef struct AudioState{
    bool initialized;
    Music bg;
    
    Sound sfx_die;
    Sound sfx_goal;
    Sound sfx_jump;

    bool bgLoaded;
    bool dieLoaded;
    bool goalLoaded;
    bool jumpLoaded;
} AudioState;

static AudioState G = {0};
static bool file_exists_multitry(const char* rel) {
    if (FileExists(rel)) return true;
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return FileExists(up);
}

static Sound load_sound_multitry(const char* rel) {
    if (FileExists(rel)) return LoadSound(rel);
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return LoadSound(up);
}

static Music load_music_multitry(const char* rel) {
    if (FileExists(rel)) return LoadMusicStream(rel);
    char up[512]; snprintf(up, sizeof(up), "../%s", rel);
    return LoadMusicStream(up);
}


bool audio_init(void) {
    if (G.initialized) return true;

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_ERROR, "[Audio] Falha ao iniciar dispositivo!");
        return false;
    }
    SetMasterVolume(MASTER_VOL);

    const char* path_bg = "assets/audio/bg_main.mp3";
    if (file_exists_multitry(path_bg)) {
        G.bg = load_music_multitry(path_bg);
        G.bg.looping = true;
        G.bgLoaded = (G.bg.ctxData != NULL);
        
        if(G.bgLoaded) {
            SetMusicVolume(G.bg, MUSIC_VOL);
            TraceLog(LOG_INFO, "[Audio] Musica carregada: %s", path_bg);
        }
    } else {
        TraceLog(LOG_WARNING, "[Audio] Arquivo de musica nao encontrado: %s", path_bg);
    }

    const char* path_die = "assets/audio/die.wav";
    const char* path_goal = "assets/audio/goal.wav";
    const char* path_jump = "assets/audio/jump.wav";

    if (file_exists_multitry(path_die)) {
        G.sfx_die = load_sound_multitry(path_die);
        G.dieLoaded = (G.sfx_die.frameCount > 0);
    }
    
    if (file_exists_multitry(path_goal)) {
        G.sfx_goal = load_sound_multitry(path_goal);
        G.goalLoaded = (G.sfx_goal.frameCount > 0);
    }
    
    if (file_exists_multitry(path_jump)) {
        G.sfx_jump = load_sound_multitry(path_jump);
        G.jumpLoaded = (G.sfx_jump.frameCount > 0);
    }

    G.initialized = true;
    return true;
}

void audio_update(void) {
    if (!G.initialized) return;
    if (G.bgLoaded && IsMusicStreamPlaying(G.bg)) {
        UpdateMusicStream(G.bg);
    }
}

void audio_play_music(void) {
    if (G.initialized && G.bgLoaded) {
        if (!IsMusicStreamPlaying(G.bg)) {
            PlayMusicStream(G.bg);
            TraceLog(LOG_INFO, "[Audio] Play Music");
        }
    }
}

void audio_stop_music(void) {
    if (G.initialized && G.bgLoaded) {
        StopMusicStream(G.bg);
        TraceLog(LOG_INFO, "[Audio] Stop Music");
    }
}

bool audio_is_music_playing(void) {
    if (!G.initialized || !G.bgLoaded) return false;
    return IsMusicStreamPlaying(G.bg);
}

void audio_toggle_music(void) {
    if (!G.initialized || !G.bgLoaded) return;
    
    if (IsMusicStreamPlaying(G.bg)) {
        PauseMusicStream(G.bg);
    } else {
        ResumeMusicStream(G.bg);
    }
}

void audio_play_event(AudioEvent e) {
    if (!G.initialized) return;

    switch (e) {
        case AUDIO_SFX_DIE:
            if (G.dieLoaded) {
                SetSoundVolume(G.sfx_die, SFX_VOL);
                PlaySound(G.sfx_die);
            }
            break;

        case AUDIO_SFX_GOAL:
            if (G.goalLoaded) {
                SetSoundVolume(G.sfx_goal, SFX_VOL);
                PlaySound(G.sfx_goal);
            }
            break;

        case AUDIO_SFX_JUMP:
            if (G.jumpLoaded) {
                float pitchVar = 0.9f + ((float)GetRandomValue(0, 20) / 100.0f);
                
                SetSoundPitch(G.sfx_jump, pitchVar);
                SetSoundVolume(G.sfx_jump, SFX_VOL * 0.9f); 
                
                if (IsSoundPlaying(G.sfx_jump)) StopSound(G.sfx_jump);
                PlaySound(G.sfx_jump);
            }
            break;
    }
}

void audio_shutdown(void) {
    if (!G.initialized) return;
    
    if (G.bgLoaded) {
        StopMusicStream(G.bg);
        UnloadMusicStream(G.bg);
    }
    if (G.dieLoaded) UnloadSound(G.sfx_die);
    if (G.goalLoaded) UnloadSound(G.sfx_goal);
    if (G.jumpLoaded) UnloadSound(G.sfx_jump);

    CloseAudioDevice();
    G.initialized = false;
    TraceLog(LOG_INFO, "[Audio] Shutdown completo.");
}