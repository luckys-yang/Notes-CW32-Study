#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"

// 引脚定义 PB1
#define DHT11_Pin GPIO_PIN_1

// 引脚模式设置(输出/输入)
#define DHT11_PIN_OUT PB01_DIR_OUTPUT()
#define DHT11_PIN_INPUT PB01_DIR_INPUT()
// 引脚拉低/拉高
#define DHT11_PIN_RESET PB01_SETLOW()
#define DHT11_PIN_SET PB01_SETHIGH()
// 读取引脚电平
#define DHT11_Read_Pin GPIO_ReadPin(CW_GPIOB,DHT11_Pin)


typedef struct
{
    float DHT11_Temperture; // 温度
    uint8_t DHT11_Humidity; // 湿度
    uint8_t (*DHT11_Init)(void);    // DHT11初始化
    uint8_t (*DHT11_Read_Data)(float *, uint8_t *);   // DNT11获取一次数据
}DHT11_t;


extern DHT11_t DHT11;

#endif
