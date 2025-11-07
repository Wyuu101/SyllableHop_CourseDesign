#ifndef __MYCLOCK_H
#define __MYCLOCK_H

#include "GUI_App.h"
#include "main.h"


extern uint8_t alarmSecond;
extern uint8_t alarmMin;
extern uint8_t alarmHour ;
extern uint8_t alarmPointCount;

extern uint8_t timeSecond;
extern uint8_t timeMin;
extern uint8_t timeHour ;
extern uint8_t timeWeek ;
extern uint8_t timeDay ;
extern uint8_t timeMon ;
extern uint8_t timeYear ;
extern uint8_t timePointCount;


void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
void DisplayAlarm(void);
void DisplayTempAlarm(void);
void SaveAlarm(void);
void RefreshSelectAlarmPointer(void);

void DisplayTempTime(void);
void SaveTime(void);
void RefreshSelectTimePointer(void);
#endif

