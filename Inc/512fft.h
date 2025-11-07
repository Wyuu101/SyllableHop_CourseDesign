#ifndef __512FFT_H
#define __512FFT_H

#include "arm_math.h"
#include "arm_const_structs.h"

//FFT 512点处理函数
void compute_fft_512(int16_t *audio_buffer, int16_t *fft_magnitude);
int findMaxIndex(int16_t *arr, int length);
float getRealMainFreq(int mainFreqIndex);
float getRealMainFreq2048(int mainFreqIndex);
#endif

