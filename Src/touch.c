#include "touch.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c3;

// 储存点击位置的函数
void checkTouch(void){
    uint8_t buf[6];
    HAL_I2C_Mem_Read(&hi2c3, 0x70, 0x02, 1, buf, 1, 1000);
    if(buf[0] > 0 && buf[0] < 6){
        HAL_I2C_Mem_Read(&hi2c3, 0x70, 0x03, 1, buf, 4, 1000);
        GUI_TOUCH_StoreState((buf[2] << 8 | buf[3]) & 0xfff,
                             (buf[0] << 8 | buf[1]) & 0xfff);
    } else {
        GUI_TOUCH_StoreState(-1, -1);
    }
}
