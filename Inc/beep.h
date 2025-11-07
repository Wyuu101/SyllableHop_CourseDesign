#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f7xx_hal.h"

typedef struct {
    uint8_t isBeeping;
    uint8_t isCalledByAlarm; // 是否是因闹钟调用
    uint32_t endTime;  // 结束时间（ms）
} Buzzer_t;

extern Buzzer_t buzzer;

void Buzzer_Beep(uint32_t freq_reload, uint32_t duration_ms,uint8_t isCalledByAlarm);


#endif

