/***************************************************************************
 * File: modbus.c
 * Author: Luckys.
 * Date: 2023/06/12
 * description: ModBus协议
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Protocol_Analysis(USART1_t*);  //协议分析
static void Modbus_Read_Register(USART1_t*);   //读寄存器
static void Modbus_Wrtie_Register(USART1_t*);  //写寄存器

/*====================================static function declaration area   END====================================*/
Modbus_t  Modbus = 
{
	1,
	
	Protocol_Analysis
};

/*
* @function: Protocol_Analysis
* @param: UART -> 串口1结构体指针
* @retval: None
* @brief: 描述
*/
static void Protocol_Analysis(USART1_t *UART)
{
    USART1_t *const COM = UART;
    uint8_t i = 0, Index = 0;

    // 过滤干扰数据，首字节为modbus地址，共8字节
    for (i = 0; i < USART1_Rec_LENGTH; i++)
    {
        // 检测键值起始数据Modbus.Addr
        if (Index == 0)
        {
            if (*(COM->puc_Usart1_Rec_Buffer + i) != Modbus.Addr)
                // 跳过下面的代码i++进行下一次循环
                continue;
        }

        *(COM->puc_Usart1_Rec_Buffer + Index) = *(COM->puc_Usart1_Rec_Buffer + i);

        // 已读取8个字节
        if (Index == Modbus_Order_LENGTH)
        {
            break;
        }

        Index++;
    }

    // 计算CRC-16
    CRC_16.CRC_Value = CRC_16.CRC_Check(COM->puc_Usart1_Rec_Buffer, 6); // 计算CRC值
    CRC_16.CRC_H = (uint8_t)(CRC_16.CRC_Value >> 8);
    CRC_16.CRC_L = (uint8_t)CRC_16.CRC_Value;

    // 校验CRC-16(为了兼容不同市面上的协议，高字节在前或者低字节在前)
    if (((*(COM->puc_Usart1_Rec_Buffer + 6) == CRC_16.CRC_L) && (*(COM->puc_Usart1_Rec_Buffer + 7) == CRC_16.CRC_H)) ||
        ((*(COM->puc_Usart1_Rec_Buffer + 6) == CRC_16.CRC_H) && (*(COM->puc_Usart1_Rec_Buffer + 7) == CRC_16.CRC_L)))
    {
        // 校验地址
        if ((*(COM->puc_Usart1_Rec_Buffer + 0)) == Modbus.Addr)
        {
            // 处理数据
            if ((*(COM->puc_Usart1_Rec_Buffer + 1)) == FunctionCode_Read_Register)
            {
                Modbus_Read_Register(COM);
            }
            else if ((*(COM->puc_Usart1_Rec_Buffer + 1)) == FunctionCode_Write_Register)
            {
                Modbus_Wrtie_Register(COM);
            }
        }
    }
    // 清缓存
    Public.Memory_Clear(COM->puc_Usart1_Rec_Buffer, USART1_Rec_LENGTH);
    USART1.ucUsart1_Rx_Cnt = 0;
}

/*
* @function: Modbus_Read_Register
* @param: UART -> 串口1结构体指针
* @retval: None
* @brief: 读寄存器
*/
static void Modbus_Read_Register(USART1_t *UART)
{
    USART1_t *const  COM = UART;

    //校验地址
    if((*(COM->puc_Usart1_Rec_Buffer + 2) == 0x9C) && (*(COM->puc_Usart1_Rec_Buffer + 3) == 0x41))
    {
        ////回应数据
        //地址码
        *(COM->puc_Usart1_Send_Buffer + 0)  = Modbus.Addr;
        //功能码
        *(COM->puc_Usart1_Send_Buffer + 1)  = FunctionCode_Read_Register;
        //数据长度
        *(COM->puc_Usart1_Send_Buffer + 2)  = 2;
        //蜂鸣器状态
        *(COM->puc_Usart1_Send_Buffer + 3)  = 0;
        *(COM->puc_Usart1_Send_Buffer + 4) = Buzzer.Buzzer_Status;

        //插入CRC
        CRC_16.CRC_Value = CRC_16.CRC_Check(COM->puc_Usart1_Send_Buffer, 5); //计算CRC值，因为CRC前有5个字节
        CRC_16.CRC_H     = (uint8_t)(CRC_16.CRC_Value >> 8);
        CRC_16.CRC_L     = (uint8_t)CRC_16.CRC_Value;
		// 低位在前高位在后
        *(COM->puc_Usart1_Send_Buffer + 5) = CRC_16.CRC_L;
        *(COM->puc_Usart1_Send_Buffer + 6) = CRC_16.CRC_H;

        //发送数据
        USART1.USART1_Send_Array(COM->puc_Usart1_Send_Buffer, 7);
        printf("\r\n"); // VOFA+上位机需要的！
    }
}

/*
* @function: Modbus_Wrtie_Register
* @param: UART -> 串口1结构体指针
* @retval: None
* @brief: 描述
*/
static void Modbus_Wrtie_Register(USART1_t *UART)
{
    USART1_t *const COM = UART;
    uint8_t i;

    ////回应数据
    // 准备数据
    for (i = 0; i < 8; i++)
    {
        *(COM->puc_Usart1_Send_Buffer + i) = *(COM->puc_Usart1_Rec_Buffer + i);
    }
    // 发送数据
    USART1.USART1_Send_Array(COM->puc_Usart1_Send_Buffer, 8);
    printf("\r\n"); // VOFA+上位机需要的！

    // 校验地址 -> 蜂鸣器
    if ((*(COM->puc_Usart1_Rec_Buffer + 2) == 0x9C) && (*(COM->puc_Usart1_Rec_Buffer + 3) == 0x44)) // 40004
    {
        // 控制蜂鸣器
        if (*(COM->puc_Usart1_Rec_Buffer + 5) == 0x01)
        {
            Buzzer.Buzzer_ON();
        }
        else
        {
            Buzzer.Buzzer_OFF();
        }
    }
}
