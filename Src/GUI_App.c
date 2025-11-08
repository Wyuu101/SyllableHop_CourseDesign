  /**
  ******************************************************************************
  * @file    GUI_App.c
  * @author  MCD Application Team
  * @brief   Simple demo drawing "Hello world"  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2018 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "GUI_App.h"
#include "GUI.h"
#include <stdio.h>
#include "DIALOG.h"
#include "touch.h"
#include "audioInputTools.h"
#include "512fft.h"
#include "myclock.h"
#include "beep.h"

extern  WM_HWIN CreateWindow(void);  
// ------------------------------波形显示功能用到的变量------------------------------
// 用于缓存时域采样点
static int16_t audio_display_buffer[340];  


// ------------------------------频谱显示功能用到的变量------------------------------
// 用于缓存时域采样点
static int16_t fft_audio_buffer[512];  
// 用于缓存FFT计算结果
static int16_t fft_magnitude[256];
// 用于控制主频率显示刷新速度的变量
int count_mainFreq_display = 0;


// ------第二类频谱显示用到的变量------
// 缓存每一帧要绘制的所有矩形的数据
extern DefRectStruct rectangles[RECT_ARRAY_SIZE];


// ------------------------------音调提取并回放功能用到的变量------------------------------
// 用于缓存时域采样点
static int16_t fft2048_audio_buffer[2048];
// 用于缓存FFT计算结果
static int16_t fft2048_magnitude[1024];
// 用于缓存每帧幅值最高的频率点
static float mainFreqs[78];
// 定时器12的句柄
extern TIM_HandleTypeDef htim12;


// ------------------------------其他变量------------------------------
// 都是用于从getAudioDisplayData()获取并缓存结果的变量
int audio_data_size = 0;
int fft_audio_buffer_size = 0;



void GRAPHICS_MainTask(void) {
  
  /* 2- Create a Window using GUIBuilder */
  // 创建主菜单窗口
  CreateWindow();
  /* USER CODE BEGIN GRAPHICS_MainTask */
  // 显示默认闹钟
  DisplayAlarm();
  /* User can implement his graphic application here */
  

  while(1)
  {  
    
    // ------------------------如果当前正在进行时域波形显示------------------------
    if(enableWaveDisplay){
        // 获取新的音频时域数据
        getAudioDisplayData(audio_display_buffer, &audio_data_size);
        if(audio_data_size > 0) {
          for(int i = 0; i < audio_data_size; i++) {
              // 将16位音频数据缩放到适合显示的范围内
              // 音频数据范围：-32768 到 32767
              // 缩放到0-100
              int display_value = (audio_display_buffer[i] + 32768) / 328;  
              // 将数据添加到GRAPH控件中进行显示
              GRAPH_DATA_YT_AddValue(hData, display_value);
          }
        }
     }
    // ------------------------如果当前正在进行频域波形显示------------------------
    else if(enableFreqWaveDisplay){
        getAudioDisplayData(fft_audio_buffer, &fft_audio_buffer_size);
        if(fft_audio_buffer_size > 0) {
          // 进行512点FFT变换
          compute_fft_512(fft_audio_buffer,fft_magnitude);
          // ------------------------如果显示的是第一类频域波形样式------------------------
          if(!enableFreqWave2){
            // 将计算好的频域数据送入GRAPH显示
            for(int i = 0; i < fft_audio_buffer_size / 2; i++) {
               int display_value = (fft_magnitude[i] + 32768) / 164 - 200; // 缩放到0-200
               GRAPH_DATA_YT_AddValue(hFreqData, display_value);
            }
            // 用于控制主频显示的刷新速度
            count_mainFreq_display ++;
            if(count_mainFreq_display >=10){
               int mainFreqIndex = findMaxIndex(fft_magnitude,256);
               float mainFreq = getRealMainFreq(mainFreqIndex);
               char mainFreqText[20];  
               sprintf(mainFreqText, "%.3f", mainFreq);
               TEXT_SetText(hFreqText,mainFreqText);
               count_mainFreq_display = 0;
            }
          }
          // ------------------------如果显示的是第二类频域波形样式------------------------
          else{
            if(rectDrawFinished){
              // 计算下一帧所有矩形的数据
              int x0,y0,x1,y1;
              for (int i = 0; i < 32; i++){
                x0 = i*10;
                x1 = i*10 + 8;
                y1 = 272;
                y0 = 0;
                int16_t maxVal = 0;
                for (int j = i*8; j < i*8 +8; j++){
                  if(fft_magnitude[j]>maxVal) maxVal = fft_magnitude[j];
                }
                int scaledVal =(int)((maxVal / 32767.0f) * 272.0f);
                y0 = 272 - scaledVal;
                rectangles[i].x0 = x0;
                rectangles[i].y0 = y0;
                rectangles[i].x1 = x1;
                rectangles[i].y1 = y1;
              }
              // 标记，表示数据计算完成，可以绘制
              rectDrawFinished = 0;
              // 触发页面的WM_PAINT回调
              WM_InvalidateWindow(hFreqWave2Win);
            }
          }
        }
     }
    // ------------------------如果当前正在进行音调提取和播放功能------------------------
    else if(enterAudioRecordPage){
       // ------------------------如果当前正在录音------------------------
       if(isRecording){
          // 获取2048点时域数据
          getAudioDisplayData(fft2048_audio_buffer, &fft_audio_buffer_size);
          if(fft_audio_buffer_size > 0) {
            // 进行2048点FFT（由于栈地址于LTDC起始地址发生冲突，无法继续增大栈空间，因此无法使用2048点FFT）
            compute_fft_512(fft2048_audio_buffer,fft2048_magnitude);
            // 找出功率最大的频点索引并记录
            int mainFreqIndex = findMaxIndex(fft2048_magnitude,1024);
            // 通过索引值计算出最大频点的真实频率值
            float mainFreq = getRealMainFreq2048(mainFreqIndex);
            mainFreqs[countFrames] = mainFreq;
            // 计算进度条显示值
            float factor = countFrames / 77.0;
            int v = (int)(factor * 100);
            float numForTextA = factor * 20;
            char TextA[20];  
            sprintf(TextA, "%.1f", numForTextA);
            TEXT_SetText(hAudioRecordTextA,TextA);
            PROGBAR_SetValue(hAudioRecordBarA,v);
            // 递增当前已记录的帧数
            countFrames ++;
            // 如果记录完成，就进行标记，使能播放按钮
            if(countFrames >= 78){
              isRecording =0;
              countFrames = 0;
              WM_EnableWindow(hAudioRecordButtonB);
            } 
          }
       }
       // ------------------------如果当前正在播放------------------------
       else if(isReplaying){
         // 取出前面记录的主频点
         float mainFreq = mainFreqs[countReplayFrames];
         // 计算定时器重装载值，用于播放不同频率的音调
         int ARQ = (int)(108000.0f / mainFreq) - 1;
         __HAL_TIM_SET_AUTORELOAD(&htim12, ARQ);
         // 设置占空比，用于调整音高
         __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, ARQ/5);  
         //  记录进度条的值
         float factor = countReplayFrames / 77.0; 
         int v = (int)(factor * 100); 
         float numForTextB = factor * 20; 
         char TextB[20]; 
         sprintf(TextB, "%.1f", numForTextB); 
         TEXT_SetText(hAudioRecordTextB,TextB); 
         PROGBAR_SetValue(hAudioRecordBarB,v); 
         // 递增已经播放的帧数值
         countReplayFrames ++; 
         // 如果播放完成，进行标记，同时使能记录和播放按钮
         if(countReplayFrames >= 78){ 
           isReplaying =0; 
           countReplayFrames = 0; 
           __HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_1,0); 
           WM_EnableWindow(hAudioRecordButtonA); 
           WM_EnableWindow(hAudioRecordButtonB);
         }
         GUI_Delay(256);
       }
     }
     // 控制基于Systick的蜂鸣器，主要用于按键音效和闹钟提示
     // 判断如果当前发声时间已足够，则执行结束发声的操作
     // 如果闹钟响起并且由用户手动关闭，也接收发声操作
     if ((buzzer.isBeeping && HAL_GetTick() > buzzer.endTime) ||(buzzer.isBeeping && buzzer.isCalledByAlarm && !isAlarmEnabled )) {
       // 占空比设为0，关闭声音
       __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, 0);
       // 修改标志位
       buzzer.isBeeping = 0;
       // 如果发声是由闹钟引起的，则顺手关闭LED灯
       if(buzzer.isCalledByAlarm){
         HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
       }
     }
     
     GUI_Delay(2);
     checkTouch();
  }
}


/* USER CODE END GRAPHICS_MainTask */


/*************************** End of file ****************************/
