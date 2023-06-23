#ifndef __KEY_1_H
#define __KEY_1_H
#include "main.h"

// 管脚 K1--PB13 K2--PB14 K3--PB15
#define Key1_Pin    GPIO_PIN_13
#define Key2_Pin    GPIO_PIN_14
#define Key3_Pin    GPIO_PIN_15

// 读取按键电平
#define READ_KEY1    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_13)
#define READ_KEY2    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)
#define READ_KEY3    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_15)

typedef enum
{
    KEY_NULL  = (uint8_t)0x00,  // 无按键按下键值
    KEY1_DOWN = (uint8_t)0x01,  // 按键1按下键值  
    KEY2_DOWN = (uint8_t)0x02,  // 按键2按下键值
    KEY3_DOWN = (uint8_t)0x03,  // 按键3按下键值
}Key_1_Status_t;

typedef struct
{
    uint16_t volatile vusKey_1_Timer_Count; // 长按计数
    uint8_t volatile vucKey_1_Flag_Arr[6];  // 按键标志位(短长按)
    void (*Key_1_Init)(void);   // 按键初始化
    void (*Key_1_Scan)(void);   // 按键三行消抖---按键扫描
    void (*Key_1_Handler)(void);    // 按键处理
}Key_1_t;

extern Key_1_t Key_1;


#endif

