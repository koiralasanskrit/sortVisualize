#include <SDL.h>
#include <stdio.h>
#include "soundsynth.h"

uint64_t soundsynth_samples_played = 0;
SDL_AudioSpec soundsynth_audio_spec_want, soundsynth_audio_spec;
SDL_AudioDeviceID soundsynth_audio_device_id;
float soundsynth_volume = 0.3;
float soundsynth_frequency = 200.0;

float soundsynth_svars_frequency[7] = {256, 280, 312, 346, 384, 426, 480};

void
soundsynth_audio_callback(void* userdata, uint8_t* stream, int len)
{
    uint64_t* samples_played = (uint64_t*)userdata;
    float* fstream = (float*)(stream);

    for(int sid = 0; sid < (len / 8); ++sid)
    {
        double time = (*samples_played + sid) / 44100.0;
        double x = 2.0 * M_PI * time * soundsynth_frequency;
        fstream[2 * sid + 0] = soundsynth_volume * sin(x); /* L */
        fstream[2 * sid + 1] = soundsynth_volume * sin(x); /* R */
    }

    *samples_played += (len / 8);
}

void
soundsynth_audio_init()
{

    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr,
            "Error initializing SDL. SDL_Error: %s\n",
            SDL_GetError()
        );
    }

    SDL_memset(&soundsynth_audio_spec_want, 0, sizeof(soundsynth_audio_spec_want));

    soundsynth_audio_spec_want.freq     = 44100;
    soundsynth_audio_spec_want.format   = AUDIO_F32;
    soundsynth_audio_spec_want.channels = 2;
    soundsynth_audio_spec_want.samples  = 512;
    soundsynth_audio_spec_want.callback = soundsynth_audio_callback;
    soundsynth_audio_spec_want.userdata = (void*)&soundsynth_samples_played;

    soundsynth_audio_device_id = SDL_OpenAudioDevice( NULL,
        0,
        &soundsynth_audio_spec_want,
        &soundsynth_audio_spec,
        SDL_AUDIO_ALLOW_FORMAT_CHANGE
    );

}

void 
soundsynth_play()
{
	SDL_PauseAudioDevice(soundsynth_audio_device_id, 0);
}

void 
soundsynth_pause()
{
	SDL_PauseAudioDevice(soundsynth_audio_device_id, 1);
}

void 
soundsynth_playsvar(int input)
{
    int index = input / 7; /* This gives the tble */
    soundsynth_frequency = (index + 1) * soundsynth_svars_frequency[input % 7];
}
