/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 公用部分代码
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Memory_Clear(uint8_t*, uint16_t);    // 内存清除
static void System_MS_Delay(uint32_t);   // 系统ms延时
static void System_10US_Delay(uint32_t); // 系统延时10*xus

/*====================================static function declaration area   END====================================*/
Public_t Public = 
{
    System_10US_Delay,
    System_MS_Delay,
    Memory_Clear,
};

/*
* @function: Memory_Clear
* @param: puc_Buffer -> 要清除的内存首地址 LEN -> 内存长度(注意字串符用strlen() 数组用sizeof() !!!)
* @retval: None
* @brief: 内存清除
*/
static void Memory_Clear(uint8_t* puc_Buffer, uint16_t LEN)
{
    uint16_t i = 0;

    for (i = 0; i < LEN; i++)
    {
        *(puc_Buffer + i) = (uint8_t)0;
    }
}

/*
* @function: fputc
* @param: ch -> 要写入的字符的 ASCII 码值，应当是一个整型数 f -> 指向要写入的文件（或流）的指针
* @retval: None
* @brief: 重定向printf
*/
int fputc(int ch, FILE *f)
{
    USART_SendData_8bit(UART_DEBUG, (uint8_t)ch);   // 发送一个数据(8bit)
    // 等待发送完成，1：完成，0：还没完成
    while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);

    return ch;
}

/*
* @function: System_MS_Delay
* @param: ms -> 需要延时的时间(ms)
* @retval: None
* @brief: 系统ms延时
*/
static void System_MS_Delay(uint32_t ms)
{
    delay1ms(ms);   // CW库的延时函数
}

/*
* @function: System_10US_Delay
* @param: us -> 需要延时的时间(us*10)
* @retval: None
* @brief: 系统10*xus延时
*/
static void System_10US_Delay(uint32_t us)
{
   delay10us(us);   // CW库的延时函数
}


