#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include "soundsynth.h"
#include "sortbase.h"

uint64_t soundsynth_samples_played = 0;
SDL_AudioSpec soundsynth_audio_spec_want, soundsynth_audio_spec;
SDL_AudioDeviceID soundsynth_audio_device_id;
float soundsynth_volume = 0.1;
float soundsynth_frequency = 200.0;

float soundsynth_svars_frequency[7] = {256, 280, 312, 346, 384, 426, 480};

struct sb_complex {
    float real, img;
};

struct data {
    int x, y;
};
typedef struct data data;
typedef struct sb_complex sb_complex;

sb_complex
soundsynth_dft_xk(int k, data* xy, int N)
{
    float sum_real = 0;
    float sum_img = 0;
    for (int i = 0; i <= k; i++)
    {
		float b_o = (2 * M_PI * k * i / N);
        sum_real += xy[i].x * cos(-b_o);
        sum_img += xy[i].x * sin(-b_o);
    }
    return (sb_complex) {sum_real, sum_img};
}

sb_complex
soundsynth_fourier(int k, data* xy, int N)
{
    float sum_real = 0;
    float sum_img = 0;
    sb_complex cmplx_temp = soundsynth_dft_xk(0, xy, N);
    sum_real += 1/2 * cmplx_temp.img;
    sum_img += 1/2 * cmplx_temp.real;
    for (int i = 1; i <= k; i++)
    {
		sb_complex cmplx_temp = soundsynth_dft_xk(i, xy, N);
		sum_real += cmplx_temp.img;
		sum_img += cmplx_temp.real;
    }
    return (sb_complex) { sum_real, sum_img };
}

float
sb_complex_mag(sb_complex sb)
{
    return sqrt((sb.real) * (sb.real) + (sb.img) * (sb.img));
}

/* Musical instruments */
float flute[] = { 10, 15, 16, 20.2, 18.2, 25, 20.1, 30, 13.5, 5, 0 };

float
soundsynth_audio_gen(x)
{
    return sin(x);
}

void
soundsynth_audio_callback(void* userdata, uint8_t* stream, int len)
{
    uint64_t* samples_played = (uint64_t*)userdata;
    float* fstream = (float*)(stream);

    for(int sid = 0; sid < (len / 8); ++sid)
    {
        double time = (*samples_played + sid) / 44100.0;
        double x = 2.0 * M_PI * time * soundsynth_frequency;
        fstream[2 * sid + 0] = soundsynth_volume * (sin(x)+cos(x)); /* L */
        fstream[2 * sid + 1] = soundsynth_volume * (sin(x)+cos(x)); /* R */
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
soundsynth_playsvar(int input, int input_for_volume)
{
    int index = input / 7; /* This gives the tble */
    soundsynth_frequency = (index + 1) * soundsynth_svars_frequency[input % 7];
}


