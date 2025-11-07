#include "myclock.h"

extern RTC_HandleTypeDef hrtc;

// ------------------------------闹钟相关变量------------------------------
uint8_t alarmSecond = 0; 
uint8_t alarmMin = 0;
uint8_t alarmHour = 0;
uint8_t alarmPointCount = 0; // 编辑模式下修改目标的索引（时:0，分:1，秒:2）


// ------------------------------时间日期相关变量------------------------------
uint8_t timeSecond = 0;
uint8_t timeMin = 0;
uint8_t timeHour  = 0;
uint8_t timeWeek  = 0;
uint8_t timeDay  = 0;
uint8_t timeMon  = 0;
uint8_t timeYear  = 0;
uint8_t timePointCount = 0; // 编辑模式下修改目标的索引（年:0，月:1，日：2，星期:3，时:4，分:5，秒:6）


// RTC每一秒会触发一次回调，回调需要执行的动作如下
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc){
// 如果当前不在编辑模式，就照常获取RTC时间并格式化为文本显示在屏幕上；如果在编辑模式，就不用显示时间文本了
if(!isEditingTime)
    {
        RTC_TimeTypeDef sTime;
        RTC_DateTypeDef sDate;
        char buf[64];  // 增大缓冲区，确保有足够的空间

        // 获取当前时间和日期
        HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);

        // 将星期几数字映射到英文缩写
        const char *weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        const char *weekDayStr = weekDays[sDate.WeekDay - 1]; // sDate.WeekDay 为 1-7，对应 Sun-Sat

        // 格式化日期和时间
        snprintf(buf, sizeof(buf), "20%02d-%02d-%02d %s %02d:%02d:%02d\r\n",
                 sDate.Year, sDate.Month, sDate.Date,
                 weekDayStr,
                 sTime.Hours, sTime.Minutes, sTime.Seconds);

        // 显示文本
        TEXT_SetText(hTimeText, buf);
    }
}


// 当时间到达设定的闹钟时，触发该回调
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
  // 点亮LED灯
  HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
  // 使蜂鸣器发声1.5秒
  Buzzer_Beep(190,1500,1);
}


// 显示闹钟时间
void DisplayAlarm(void)
{
    char alarm_str[16];    
    RTC_AlarmTypeDef sAlarm;
    // 读取闹钟A的时间
    HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
    // 格式化为字符串
    snprintf(alarm_str, sizeof(alarm_str),
             "%02d:%02d:%02d",
             sAlarm.AlarmTime.Hours,
             sAlarm.AlarmTime.Minutes,
             sAlarm.AlarmTime.Seconds);
    
    TEXT_SetText(hAlarmText,alarm_str);
}


// 用于编辑模式下显示未保存的闹钟值
void DisplayTempAlarm(void){
    char str[16];
    snprintf(str, sizeof(str),
             "%02d:%02d:%02d",
             alarmHour,
             alarmMin,
             alarmSecond);
    
    TEXT_SetText(hAlarmText,str);

}


// 用于编辑模式下显示未保存的日期、时间值
void DisplayTempTime(void){
    char str[64];
    const char *weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *weekDayStr = weekDays[timeWeek]; // sDate.WeekDay 为 1-7，对应 Sun-Sat
    // 格式化日期和时间
    snprintf(str, sizeof(str), "20%02d-%02d-%02d %s %02d:%02d:%02d\r\n",
             timeYear, timeMon, timeDay,
             weekDayStr,
             timeHour, timeMin, timeSecond);

    // 显示文本
    TEXT_SetText(hTimeText, str);
}



// 保存闹钟设置
void SaveAlarm(void){
    RTC_AlarmTypeDef sAlarm;
    HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
    sAlarm.AlarmTime.Hours = alarmHour;
    sAlarm.AlarmTime.Minutes = alarmMin;
    sAlarm.AlarmTime.Seconds = alarmSecond;
    // 先关闭闹钟才能更新设置
    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
    HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}


//  保存日期、时间设置
void SaveTime(void){
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    // 获取当前时间和日期
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    sTime.Hours = timeHour;
    sTime.Minutes = timeMin;
    sTime.Seconds = timeSecond;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    sDate.WeekDay = timeWeek+1;
    sDate.Month = timeMon;
    sDate.Date = timeDay;
    sDate.Year = timeYear;
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    
    
}

// 编辑闹钟模式下显示当前用户选中修改项的指针
void RefreshSelectAlarmPointer(void){
  switch (alarmPointCount){
    case 0:
      TEXT_SetText(hAlarmPointer,"^            ");
      break;
    case 1:
      TEXT_SetText(hAlarmPointer,"^");
      break;
    case 2:
      TEXT_SetText(hAlarmPointer,"            ^");
      break;
  }
}

// 编辑时间模式下显示当前用户选中修改项的指针
void RefreshSelectTimePointer(void){
  switch (timePointCount){
    case 0:
      TEXT_SetText(hTimePointer,"^                                    ");
      break;
    case 1:
      TEXT_SetText(hTimePointer,"       ^                             ");
      break;
    case 2:
      TEXT_SetText(hTimePointer,"            ^                        ");
      break;
    case 3:
      TEXT_SetText(hTimePointer,"                  ^                  ");
      break;
    case 4:
      TEXT_SetText(hTimePointer,"                        ^            ");
      break;
    case 5:
      TEXT_SetText(hTimePointer,"                             ^       ");
      break;
    case 6:
      TEXT_SetText(hTimePointer,"                                  ^  ");
      break;
  }
}