#ifndef __KEY_2_H
#define __KEY_2_H
#include "main.h"

// 管脚 SW1--PB2
#define SW1_Pin    GPIO_PIN_2

// 读取按键电平
#define READ_SW1    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_2)


typedef struct
{
    void (*Key_2_Init)(void);   // 按键初始化
}Key_2_t;

extern Key_2_t Key_2;


#endif
