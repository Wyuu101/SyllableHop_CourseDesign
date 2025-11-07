#ifndef __AUDIOINPUTTOOLS_H
#define __AUDIOINPUTTOOLS_H

#include "main.h"

// 音频相关定义
#define AUDIO_BUFFER_SIZE 340  

#define FFT_AUDIO_BUFFER_SIZE 512

#define FFT_AUDIO_RECORD_BUFFER_SIZE 2048


void getAudioDisplayData(int16_t* buffer, int* size);  // 传递音频采样数据
void AudioInputInit(void);  // 初始化音频输入
void AudioInputStop(void); // 停止音频信号输入采样
#endif


