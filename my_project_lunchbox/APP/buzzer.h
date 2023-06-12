#ifndef __BUZZER_H
#define __BUZZER_H
#include "main.h"

// 管家定义 buzzer--PB3
#define Buzzer_Pin GPIO_PIN_3

typedef enum
{
    Buzzer_Status_ON = (uint8_t)0x01,   // 蜂鸣器打开中态
    Buzzer_Status_OFF = (uint8_t)0x00,  // 蜂鸣器关闭态
}Buzzer_Status_t;

typedef struct
{
    Buzzer_Status_t Buzzer_Status;    // 蜂鸣器状态
    void (*Buzzer_Init)(void);  // 蜂鸣器初始化
    void (*Buzzer_ON)(void);  // 蜂鸣器打开
    void (*Buzzer_OFF)(void);  // 蜂鸣器关闭
}Buzzer_t;

extern Buzzer_t Buzzer;

#endif
