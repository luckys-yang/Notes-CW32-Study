#ifndef __LED_H
#define __LED_H
#include "main.h"

// 管脚 LED1--PA7 LED2--PA8 LED--PC13
#define Led1_Pin    GPIO_PIN_7
#define Led2_Pin    GPIO_PIN_8
#define Led3_Pin    GPIO_PIN_13

// 定义枚举类型
typedef enum
{
    LED1 = (uint8_t)0x01,
    LED2 = (uint8_t)0x02,
    LED3 = (uint8_t)0x03,
} Led_Num_t;

typedef struct
{
    void (*Led_Init)(void); // LED初始化
    void (*Led_ON)(Led_Num_t);   // 打开
    void (*Led_OFF)(Led_Num_t);  // 关闭
    void (*Led_Flip)(Led_Num_t); // 翻转
} Led_t;

extern Led_t Led;

#endif

