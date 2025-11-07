#include "beep.h"

extern TIM_HandleTypeDef htim12;

// 结构体
Buzzer_t buzzer = {0};

void Buzzer_Beep(uint32_t freq_reload, uint32_t duration_ms,uint8_t isCalledByAlarm)
{   
    // 设置重装载值，用于控制频率
    __HAL_TIM_SET_AUTORELOAD(&htim12, freq_reload);
    // 设置占空比，用于设置音量
    __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, freq_reload / 5);
    buzzer.isBeeping = 1;
    buzzer.isCalledByAlarm = isCalledByAlarm;
    buzzer.endTime = HAL_GetTick() + duration_ms;
}