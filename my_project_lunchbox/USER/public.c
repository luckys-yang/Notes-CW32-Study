/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ���ò��ִ���
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Memory_Clear(uint8_t*, uint16_t);    // �ڴ����
static void System_MS_Delay(uint32_t);   // ϵͳms��ʱ
static void System_10US_Delay(uint32_t); // ϵͳ��ʱ10*xus

/*====================================static function declaration area   END====================================*/
Public_t Public = 
{
    System_10US_Delay,
    System_MS_Delay,
    Memory_Clear,
};

/*
* @function: Memory_Clear
* @param: puc_Buffer -> Ҫ������ڴ��׵�ַ LEN -> �ڴ泤��(ע���ִ�����strlen() ������sizeof() !!!)
* @retval: None
* @brief: �ڴ����
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
* @param: ch -> Ҫд����ַ��� ASCII ��ֵ��Ӧ����һ�������� f -> ָ��Ҫд����ļ�����������ָ��
* @retval: None
* @brief: �ض���printf
*/
int fputc(int ch, FILE *f)
{
    USART_SendData_8bit(UART_DEBUG, (uint8_t)ch);   // ����һ������(8bit)
    // �ȴ�������ɣ�1����ɣ�0����û���
    while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);

    return ch;
}

/*
* @function: System_MS_Delay
* @param: ms -> ��Ҫ��ʱ��ʱ��(ms)
* @retval: None
* @brief: ϵͳms��ʱ
*/
static void System_MS_Delay(uint32_t ms)
{
    delay1ms(ms);   // CW�����ʱ����
}

/*
* @function: System_10US_Delay
* @param: us -> ��Ҫ��ʱ��ʱ��(us*10)
* @retval: None
* @brief: ϵͳ10*xus��ʱ
*/
static void System_10US_Delay(uint32_t us)
{
   delay10us(us);   // CW�����ʱ����
}


