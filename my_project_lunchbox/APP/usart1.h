#ifndef __USART1_H
#define __USART1_H
#include "main.h"

// 串口发送长度，接收长度
#define USART1_Send_LENGTH 20
#define USART1_Rec_LENGTH  100

// 串口1引脚 PB9(RX) PB8(TX)
#define Usart1_Tx_Pin GPIO_PIN_8
#define Usart1_Rx_Pin GPIO_PIN_9

typedef struct
{
    uint8_t ucUsart1_Rx_Cnt;   // 接收长度计数
    uint8_t* puc_Usart1_Send_Buffer;    // 发送缓存指针
    uint8_t* puc_Usart1_Rec_Buffer; // 接收缓存指针
    void (*USART1_Init)(void);   // 串口1初始化
    void (*USART1_Send_Array)(uint8_t*, uint16_t);  // 发送数组
    void (*USART1_Send_String)(uint8_t*);   // 发送字符串
}USART1_t;

extern USART1_t USART1;


#endif
