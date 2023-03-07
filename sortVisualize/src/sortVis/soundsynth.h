#pragma once
#include <SDL.h>
enum soundsynth_svars {
    SA, RE, GA, MA, PA, DHA, NI
};


extern float soundsynth_svars_frequency[7];
extern SDL_AudioSpec soundsynth_audio_spec_want, soundsynth_audio_spec;
extern SDL_AudioDeviceID soundsynth_audio_device_id;

extern float soundsynth_volume;
extern float soundsynth_frequency;

void soundsynth_audio_callback(void* userdata, uint8_t* stream, int len);

void soundsynth_audio_init();

void soundsynth_play();

void soundsynth_pause();

void soundsynth_playsvar(int input, int input_for_volume);
