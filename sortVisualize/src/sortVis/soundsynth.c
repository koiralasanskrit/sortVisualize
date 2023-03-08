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


/* Fourier */
#define PI 3.1415926538979
#define SIZE(array) sizeof(array) / sizeof(array[0])
#define SAMPLES 8

struct ssy_complex { float real, img; };
struct ssy_mag_angle { float mag, angle; };
typedef struct ssy_complex ssy_complex;
typedef struct ssy_mag_angle ssy_mag_angle;


ssy_complex
ssy_xk(int kth_coefficient, int no_of_samples, float *samples)
{
    float sum_img = 0;
    float sum_real = 0;
    for (int index = 0; index < no_of_samples; index++)
    {
        float x_o = samples[index];
        float b_o = 2 * PI * kth_coefficient * index / no_of_samples;
        sum_img += x_o * sin(-b_o);
        sum_real += x_o * cos(-b_o);
        //        printf("x_o:%f kth:%f sin(-b_o):%f\n", x_o, kth_coefficient, sin(-b_o));
    }
    return (ssy_complex){sum_real, sum_img};
}

ssy_complex
inverse_fourier(int kth_coefficient, int no_of_samples, ssy_complex *samples)
{
    float sum_img = 0;
    float sum_real = 0;
    for (int index = 0; index < no_of_samples; index++)
    {
        ssy_complex x_o = samples[index];
        float b_o = 2 * PI * kth_coefficient * index / no_of_samples;
        sum_img += x_o.img * sin(b_o);
        sum_real += x_o.real * cos(b_o);
        // printf("x_o:%f kth:%f sin(-b_o):%f\n", x_o, kth_coefficient, sin(-b_o));
    }
    return (ssy_complex){sum_real * 1 / no_of_samples, sum_img * 1 / no_of_samples};
}

ssy_complex
fourier_sum(int no_of_samples, float *samples)
{
    ssy_complex sum = (ssy_complex){0, 0};
    for (int i = 0; i < no_of_samples; i++)
    {
        ssy_complex s = ssy_xk(i, no_of_samples, samples);
        sum.img += s.img;
        sum.real += s.real;
    }
    return sum;
}

float
ssy_mag(ssy_complex cmp)
{
    return sqrt(cmp.real * cmp.real + cmp.img * cmp.img);
}

float
ssy_angle(ssy_complex cmp)
{
    return atan2f(cmp.img, cmp.real);
}

float
fourier_sum_magnitude_sampled(int no_of_samples, float *samples)
{
    float sum = 0;
    for (int i = 0; i < no_of_samples; i++)
    {
        sum += ssy_mag(ssy_xk(i, SAMPLES, samples));
    }
    return sum / no_of_samples;
}

/* Musical instruments */

struct ssy_instrument {
    float samples[SAMPLES];
    ssy_mag_angle transformed[SAMPLES/2];
    char name[30];
};
typedef struct ssy_instrument ssy_instrument;
typedef enum instrument instrument;

ssy_instrument instruments[NO_OF_INSTRUMENTS];
char instrument_strings[NO_OF_INSTRUMENTS][20] = { "Timber1", "Timber2", "Timber3"};
float instrument_samples[NO_OF_INSTRUMENTS][SAMPLES] = { 
    {0.5, 0.707, 1, 0.7, 0.6, -0.65, -0, -0.5}, 
    {-1, -1, -1, 0.0, 1, 1, 1, 1},
    {0, 0, 0, -1, 1, 1, 1 -1, 1}
};
instrument selected = TIMBER2;


void
ssy_transform_freq_domain(ssy_instrument* inst, int instr)
{
    ssy_complex out_samp[SAMPLES];
    for (int i = 0; i < 8; i++)
    {
        ssy_complex cmp = ssy_xk(i, SAMPLES, instrument_samples[instr]);
        out_samp[i] = cmp;
    }

    ssy_complex freq_domain[SAMPLES];
    for (int i = 0; i < 8 / 2; i++)
    {
        freq_domain[i].img = 2 * out_samp[i].img / 8;
        freq_domain[i].real = 2 * out_samp[i].real / 8;
        inst->transformed[i].mag = ssy_mag(freq_domain[i]);
        inst->transformed[i].angle = ssy_angle(freq_domain[i]);
    }
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

		float sum = 0;
		for (int i = 0; i < SAMPLES / 2; i++) {
			sum += instruments[selected].transformed[i].mag * cos(x * instruments[selected].transformed[i].angle);
		}

		fstream[2 * sid + 0] = soundsynth_volume * sum;
		fstream[2 * sid + 1] = soundsynth_volume * sum;
    }

    *samples_played += (len / 8);
}

void
soundsynth_audio_init()
{
    for (int i = 0; i < NO_OF_INSTRUMENTS; i++)
    {
        ssy_transform_freq_domain(&instruments[i], i);
    }

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


