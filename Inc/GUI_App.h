  /**
  ******************************************************************************
  * @file    GUI_App.h
  * @author  MCD Application Team
  * @brief   Header for GUI_App.c   
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
#include "main.h"
#include "GUIDRV_Lin.h"
/* USER CODE BEGIN Includes */
#include "DIALOG.h"
/* USER CODE END Includes */
#ifndef GUI_APP_H
#define GUI_APP_H
/* USER CODE BEGIN PV */

#define RECT_ARRAY_SIZE 32

typedef struct {
    int x0;
    int y0;
    int x1;
    int y1;
} DefRectStruct;
extern DefRectStruct rectangles[RECT_ARRAY_SIZE];
extern int rectDrawFinished;
extern WM_HWIN hWin;
extern WM_HWIN hWaveWin;
extern WM_HWIN hFreqWaveWin;
extern WM_HWIN hFreqWave2Win;
extern WM_HWIN hFreqText;
extern WM_HWIN hAudioRecordWin;
extern WM_HWIN hAudioRecordTextA;
extern WM_HWIN hAudioRecordTextB;
extern WM_HWIN hAudioRecordButtonA;
extern WM_HWIN hAudioRecordButtonB;

extern WM_HWIN hMenu1;
extern WM_HWIN hMenu2;
extern WM_HWIN hMenu3;
extern WM_HWIN hMenu4;
extern WM_HWIN hMenu5;

extern WM_HWIN hSetTimeBtn;
extern WM_HWIN hSetAlarmBtn;
extern WM_HWIN hAlarmSwitch;

extern WM_HWIN hTimeText;
extern WM_HWIN hAlarmText;
extern WM_HWIN hTimePointer;
extern WM_HWIN hAlarmPointer;
extern GRAPH_DATA_Handle hData;
extern GRAPH_DATA_Handle hFreqData;
extern PROGBAR_Handle hAudioRecordBarA;
extern PROGBAR_Handle hAudioRecordBarB;
extern int enableWaveDisplay;
extern int enableFreqWaveDisplay;
extern int enableFreqWave2;
extern int enterAudioRecordPage;
extern int isRecording;
extern int isReplaying;
extern int countFrames;
extern int countReplayFrames;
extern int isEditingTime;
extern int isEditingAlarm;
extern int isAlarmEnabled;
WM_HWIN CreateWindow(void);
WM_HWIN CreateWaveWindow(void);
WM_HWIN CreateFreqWaveWindow(void);
WM_HWIN CreateFreqWave2Window(void);
WM_HWIN CreateAudioRecordWindow(void);
/* USER CODE END PV */
void GRAPHICS_MainTask(void);

#endif /* GUI_APP_H */

/*************************** End of file ****************************/
