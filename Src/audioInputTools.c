#include "audioInputTools.h"
#include "stm32746g_discovery_audio.h"
#include "GUI_App.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdio.h>
#include "stm32f7xx_hal.h"

// 音频相关全局变量
static int16_t audio_input_buffer[AUDIO_BUFFER_SIZE * 2]; 
static int16_t audio_buffer_A[AUDIO_BUFFER_SIZE];       
static int16_t audio_buffer_B[AUDIO_BUFFER_SIZE]; 
static int16_t *point_active_buffer = audio_buffer_A;
static int16_t *point_ready_buffer = audio_buffer_B;


static int16_t audio_fft_input_buffer[FFT_AUDIO_BUFFER_SIZE * 2]; 
static int16_t audio_fft_buffer_A[FFT_AUDIO_BUFFER_SIZE];       
static int16_t audio_fft_buffer_B[FFT_AUDIO_BUFFER_SIZE];   
static int16_t *point_active_fft_buffer = audio_fft_buffer_A;
static int16_t *point_ready_fft_buffer = audio_fft_buffer_B;

static int16_t audio_record_fft_input_buffer[FFT_AUDIO_RECORD_BUFFER_SIZE * 2]; 
static int16_t audio_record_fft_buffer_A[FFT_AUDIO_RECORD_BUFFER_SIZE];       
static int16_t audio_record_fft_buffer_B[FFT_AUDIO_RECORD_BUFFER_SIZE];   
static int16_t *point_active_2048fft_buffer = audio_record_fft_buffer_A;
static int16_t *point_ready_2048fft_buffer = audio_record_fft_buffer_B;



volatile int audio_data_ready;                           
volatile int audio_fft_data_ready;       
volatile int audio_record_fft_data_ready;


// 获取音频显示数据
void getAudioDisplayData(int16_t* buffer, int* size)
{
  if(enableWaveDisplay){
      if(audio_data_ready) {
          memcpy(buffer,point_ready_buffer,AUDIO_BUFFER_SIZE*sizeof(int16_t));
          *size = AUDIO_BUFFER_SIZE;
          audio_data_ready = 0; 
      } else {
          *size = 0;
      }
  
  }
  else if(enableFreqWaveDisplay){
      if(audio_fft_data_ready) {
          memcpy(buffer,point_active_fft_buffer,FFT_AUDIO_BUFFER_SIZE*sizeof(int16_t));
          *size = FFT_AUDIO_BUFFER_SIZE; 
          audio_fft_data_ready = 0;  

      }else {
          *size = 0;
      }
  }
  else if (enterAudioRecordPage){
    if(audio_record_fft_data_ready) {
          memcpy(buffer,point_active_2048fft_buffer,FFT_AUDIO_RECORD_BUFFER_SIZE*sizeof(int16_t));
          *size = FFT_AUDIO_RECORD_BUFFER_SIZE; 
          audio_record_fft_data_ready = 0;  

      }else {
          *size = 0;
      }
  }
  else{}
}

// 初始化音频输入
void AudioInputInit(void)
{
    
    if(enableWaveDisplay){
      audio_data_ready = 0;
      // 初始化显示缓冲区
      for(int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
          audio_buffer_A[i] = 0;
          audio_buffer_B[i] = 0;
      }
      //选择U20、U21麦克风接口输入，采样率16kHz，数据宽度16位 
      BSP_AUDIO_IN_Init(SAI_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 1); 
      //开启输入采集 
      BSP_AUDIO_IN_Record((uint16_t*)audio_input_buffer, AUDIO_BUFFER_SIZE*2);
    }
    else if(enableFreqWaveDisplay){
      // 初始化显示缓冲区
      for(int i = 0; i < FFT_AUDIO_BUFFER_SIZE; i++) {
          audio_fft_buffer_A[i] = 0;
          audio_fft_buffer_B[i] = 0;
      }
     //选择U20、U21麦克风接口输入，采样率48kHz，数据宽度16位 
      BSP_AUDIO_IN_Init(SAI_AUDIO_FREQUENCY_48K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 1);
      //开启输入采集 
      BSP_AUDIO_IN_Record((uint16_t*)audio_fft_input_buffer, FFT_AUDIO_BUFFER_SIZE*2);
    }
    else if(enterAudioRecordPage){
       // 初始化显示缓冲区
      for(int i = 0; i < FFT_AUDIO_RECORD_BUFFER_SIZE; i++) {
          audio_record_fft_buffer_A[i] = 0;
          audio_record_fft_buffer_B[i] = 0;
      }
     //选择U20、U21麦克风接口输入，采样率8kHz，数据宽度16位 
      BSP_AUDIO_IN_Init(SAI_AUDIO_FREQUENCY_8K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 1);
      //开启输入采集 
      BSP_AUDIO_IN_Record((uint16_t*)audio_record_fft_input_buffer, FFT_AUDIO_RECORD_BUFFER_SIZE*2);
    }
    else{}
    
    
}


void AudioInputStop(void){
  // 停止音频信号采集
  BSP_AUDIO_IN_Stop(CODEC_PDWN_HW);
}


// 音频输入完成回调函数 - 修改为处理时域波形显示
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  if(enableWaveDisplay){
      // 处理左声道数据
      for(int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
          int16_t sample = audio_input_buffer[i * 2];
          point_active_buffer[i] = sample;
         
      }
      // 交换缓冲区
      int16_t *temp = point_ready_buffer;
      point_ready_buffer = point_active_buffer;
      point_active_buffer = temp;
      
      // 设置数据就绪标志
      audio_data_ready = 1;
      // 重新开始录音
      BSP_AUDIO_IN_Record((uint16_t*)audio_input_buffer, AUDIO_BUFFER_SIZE * 2);
  }
  else if(enableFreqWaveDisplay){
    // 处理左声道数据（假设为单声道或使用左声道）
      for(int i = 0; i < FFT_AUDIO_BUFFER_SIZE; i++) {
          // 取左声道数据
          int16_t sample = audio_fft_input_buffer[i * 2];
          point_active_fft_buffer[i] = sample; 
      }
       // 交换缓冲区
      int16_t *temp = point_ready_fft_buffer;
      point_ready_fft_buffer = point_active_fft_buffer;
      point_active_fft_buffer = temp;
      // 设置数据就绪标志
      audio_fft_data_ready = 1;
      // 重新开始录音
      BSP_AUDIO_IN_Record((uint16_t*)audio_fft_input_buffer, FFT_AUDIO_BUFFER_SIZE * 2);
  }
  else if(enterAudioRecordPage){
    // 处理左声道数据（假设为单声道或使用左声道）
      for(int i = 0; i < FFT_AUDIO_RECORD_BUFFER_SIZE; i++) {
          // 取左声道数据
          int16_t sample = audio_record_fft_input_buffer[i * 2];
          point_active_2048fft_buffer[i] = sample; 
      }
       // 交换缓冲区
      int16_t *temp = point_ready_2048fft_buffer;
      point_ready_2048fft_buffer = point_active_2048fft_buffer;
      point_active_2048fft_buffer = temp;
      // 设置数据就绪标志
      audio_record_fft_data_ready = 1;
      // 重新开始录音
      BSP_AUDIO_IN_Record((uint16_t*)audio_record_fft_input_buffer, FFT_AUDIO_RECORD_BUFFER_SIZE * 2);
  }
  else{}
}

