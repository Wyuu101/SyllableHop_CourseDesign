#include "512fft.h"
// 所有大数组都使用静态存储
static arm_rfft_fast_instance_f32 fft_instance;
static float32_t input_f32[512];
static float32_t fft_output[512];
static uint8_t fft_init_flag = 0;
static const float beginFreq = 46.875;
static const float stepFreq = 93.75;
static const float beginFreq2048 = 1.953;
static const float stepFreq2048 = 3.906;



void compute_fft_512(int16_t *audio_buffer, int16_t *fft_magnitude)
{
    float32_t mag;
    uint16_t i;

    if (!fft_init_flag) {
        arm_rfft_fast_init_f32(&fft_instance, 512);
        fft_init_flag = 1;
    }

    // int16_t → float32_t 并归一化
    for (i = 0; i < 512; i++) {
        input_f32[i] = (float32_t)audio_buffer[i] / 32768.0f;
    }

    // 执行FFT
    arm_rfft_fast_f32(&fft_instance, input_f32, fft_output, 0);

    // 计算正频率幅值
    for (i = 0; i < 256; i++) {
        float32_t real = fft_output[2 * i];
        float32_t imag = fft_output[2 * i + 1];
        mag = sqrtf(real * real + imag * imag);
        
        if (mag > 1.0f) mag = 1.0f;
        fft_magnitude[i] = (int16_t)(mag * 32767.0f);
    }
}

int findMaxIndex(int16_t *arr, int length) {
    if (length <= 0) return -1; // 防止空数组

    int16_t *ptr = arr;
    int16_t *maxPtr = ptr;

    for (int i = 1; i < length; i++) {
        ptr++;
        if (*ptr > *maxPtr) {
            maxPtr = ptr;
        }
    }

    return (int)(maxPtr - arr); // 返回最大值的索引
}

float getRealMainFreq(int mainFreqIndex){
   return (beginFreq + mainFreqIndex * stepFreq);
}



float getRealMainFreq2048(int mainFreqIndex){
   return (beginFreq2048 + mainFreqIndex * stepFreq2048);
}

