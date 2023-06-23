#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "main.h"

// Debug Port(Default serial port 1)
#define UART_DEBUG  CW_UART1

/***********************全局宏预编译 BEGIN***********************/

// 选择屏幕(只能3选1)
// #define USE_TFT 
#define USE_096_OLED
// #define USE_091_OLED

// ADC选择模式(单通道单次/序列扫描)
// #define USE_ADC_Single_One
#define USE_ADC_Serial_Scan

// RTC中断选择打开
// #define USE_RTC_Interrupt

// 输入捕获有问题，暂时屏蔽
// #define USE_PWM_IC
// #define USE_PWM_IC_ATIM

// GTIM3互补输出
// #define USE_GTIM3_TOGG

// ATIM输出比较
// #define USE_ATIM_PWM

/***********************全局宏预编译   END***********************/


// 取消 FALSE 和 TRUE 宏定义(否则下面枚举报错！)
#undef FALSE
#undef TRUE

// BIT
typedef enum
{
    BIT0 = (uint8_t)(0x01 << 0),    // 0x01 -- 0000 0001
    BIT1 = (uint8_t)(0x01 << 1),    // 0x02 -- 0000 0010
    BIT2 = (uint8_t)(0x01 << 2),    // 0x04 -- 0000 0100    
    BIT3 = (uint8_t)(0x01 << 3),    // 0x08 -- 0000 1000
    BIT4 = (uint8_t)(0x01 << 4),    // 0x10 -- 0001 0000
    BIT5 = (uint8_t)(0x01 << 5),    // 0x20 -- 0010 0000
    BIT6 = (uint8_t)(0x01 << 6),    // 0x40 -- 0100 0000
    BIT7 = (uint8_t)(0x01 << 7),    // 0x80 -- 1000 0000
}BIT_t;

// TRUE/FALSE
typedef enum
{
    FALSE = 0U,
    TRUE = !FALSE 
}FLagStatus_t;

// 超时时间
typedef enum
{
    UART_TX_TimerOut = (uint8_t)100,    // 串口发送单字节等待最大时间(ms)
}TIMER_OUT_t;

typedef struct
{
    void (*System_10US_Delay)(uint32_t);    // 系统延时10*xus
    void (*System_MS_Delay)(uint32_t);  // 系统ms延时
    void (*Memory_Clear)(uint8_t*, uint16_t);   // 内存清除
}Public_t;

extern Public_t Public;

#endif
