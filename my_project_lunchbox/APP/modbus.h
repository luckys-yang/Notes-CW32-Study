#ifndef __MODBUS_H
#define __MODBUS_H
#include "main.h"

// 定义---读写寄存器功能号，命令长度
#define FunctionCode_Read_Register 		(uint8_t)0x03
#define FunctionCode_Write_Register 	(uint8_t)0x06
#define Modbus_Order_LENGTH           (uint8_t)8

typedef struct
{
    uint16_t Addr; // 地址

    void (*Protocol_Analysis)(USART1_t *); // 协议分析
} Modbus_t;

extern Modbus_t Modbus;

#endif
