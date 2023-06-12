#ifndef __PUBLIC_H
#define __PUBLIC_H
#include "main.h"

// Debug Port(Default serial port 1)
#define UART_DEBUG  CW_UART1
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

typedef struct
{
    void (*Memory_Clear)(uint8_t*, uint16_t);   // 内存清除
}Public_t;

extern Public_t Public;

#endif
