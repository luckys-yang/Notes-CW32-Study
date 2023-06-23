/***************************************************************************
 * File: usart1.c
 * Author: Luckys.
 * Date: 2023/06/12
 * description: USART1
 -----------------------------------
USART1:TX--PB8 RX--PB9
���ߣ�
    ��Ƭ��PB8 ---- Wch-link RXD
    ��Ƭ��PB9 ---- Wch-link TXD
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/

static uint8_t Usart1_Send_Bufferp[USART1_Send_LENGTH] = {0x00}; // ��������
static uint8_t Usaer1_Rec_Buffer[USART1_Rec_LENGTH] = {0x00};    // ��������

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void USART1_Init(void);  // ����1��ʼ��
static void USART1_Send_Array(uint8_t*, uint16_t);  // ��������
static void USART1_Send_String(uint8_t*);   // �����ַ���

/*====================================static function declaration area   END====================================*/

USART1_t USART1 = 
{
    0,
    Usart1_Send_Bufferp,
    Usaer1_Rec_Buffer,
    USART1_Init,
    USART1_Send_Array,
    USART1_Send_String,
};

/*
* @function: USART1_Init
* @param: None
* @retval: None
* @brief: ����1��ʼ��
*/
static void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	//����ṹ��
    USART_InitTypeDef USART_InitStructure;	//����ṹ��

    __RCC_GPIOB_CLK_ENABLE();   // ʹ������ʱ��
    __RCC_UART1_CLK_ENABLE();   // ʹ�ܴ���ʱ��
    // �򿪸���
    PB08_AFx_UART1TXD();
    PB09_AFx_UART1RXD();
    /*�˿ڳ�ʼ��*/
    GPIO_InitStructure.IT = GPIO_IT_NONE; // �ܽ��ж�ģʽ--��
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // ģʽ--�������
    GPIO_InitStructure.Pins = Usart1_Tx_Pin;  
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  // ģʽ--��������
    GPIO_InitStructure.Pins = Usart1_Rx_Pin;   
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);  

    /*���ڳ�ʼ��*/
    CW_UART1->CR1_f.SYNC = 0;   // 0-�첽ȫ˫��ͨ��ģʽ; 1-ͬ����˫��ͨ��ģʽ
    CW_UART1->ICR = 0x00;   // ������д����жϱ�־
    USART_InitStructure.USART_BaudRate = 9600;  // ������
    USART_InitStructure.USART_Over = USART_Over_16;	// ������ʽ---16������
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��ֹӲ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ  
    // ����3����CW���е�    
    USART_InitStructure.USART_UclkFreq = 64000000;  // ����ʱ��UCLKƵ�� Hz
    USART_InitStructure.USART_StartBit = USART_StartBit_FE; // ��ʼλ�ж���ʽ---�½���(�͹�����ѡ��͵�ƽ)
    USART_InitStructure.USART_Source = USART_Source_PCLK;   // ����ʱ��ԴUCLK---PCLK(��ʱ����)
    USART_Init(CW_UART1,&USART_InitStructure);  // ���ڳ�ʼ��
    
    /*NVIC��ʼ��(����Ҫ����ṹ��)*/
    NVIC_SetPriority(UART1_IRQn,0); // ���ȼ��������ȼ�����
    NVIC_EnableIRQ(UART1_IRQn); // ʹ���ж�---�ж�ͨ����

    USART_ITConfig(CW_UART1,USART_IT_RC,ENABLE);    // ��������ж�ʹ��
}

/*
* @function: USART1_Send_Array
* @param: p_Arr -> ���������� LEN -> ���鳤��(ʹ��sizeof����)
* @retval: None
* @brief: ��������
*/
static void USART1_Send_Array(uint8_t* p_Arr, uint16_t LEN)
{
    uint16_t i;

    for (i = 0; i < LEN; i++)
    {
        USART_SendData_8bit(CW_UART1,*(p_Arr + i));
        // �ȴ�������ɣ�1����ɣ�0����û���
        while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);
    }
    while(USART_GetFlagStatus(CW_UART1, USART_FLAG_TXBUSY) == SET); // �ȴ����ڿ���
}

/*
* @function: USART1_Send_String
* @param: p_Str -> �������ַ���
* @retval: None
* @brief: �����ַ���
*/
static void USART1_Send_String(uint8_t* p_Str)
{
    while (*p_Str)
    {
        USART_SendData_8bit(CW_UART1,*p_Str);
        // �ȴ�������ɣ�1����ɣ�0����û���
        while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);
        p_Str++;
    }
    while(USART_GetFlagStatus(CW_UART1, USART_FLAG_TXBUSY) == SET); // �ȴ����ڿ���
}
