/***************************************************************************
 * File: usart1.c
 * Author: Luckys.
 * Date: 2023/06/12
 * description: USART1
 -----------------------------------
USART1:TX--PB8 RX--PB9
接线：
    单片机PB8 ---- Wch-link RXD
    单片机PB9 ---- Wch-link TXD
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/

static uint8_t Usart1_Send_Bufferp[USART1_Send_LENGTH] = {0x00}; // 发送数组
static uint8_t Usaer1_Rec_Buffer[USART1_Rec_LENGTH] = {0x00};    // 接收数据

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void USART1_Init(void);  // 串口1初始化
static void USART1_Send_Array(uint8_t*, uint16_t);  // 发送数组
static void USART1_Send_String(uint8_t*);   // 发送字符串

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
* @brief: 串口1初始化
*/
static void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	//定义结构体
    USART_InitTypeDef USART_InitStructure;	//定义结构体

    __RCC_GPIOB_CLK_ENABLE();   // 使能引脚时钟
    __RCC_UART1_CLK_ENABLE();   // 使能串口时钟
    // 打开复用
    PB08_AFx_UART1TXD();
    PB09_AFx_UART1RXD();
    /*端口初始化*/
    GPIO_InitStructure.IT = GPIO_IT_NONE; // 管脚中断模式--无
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // 模式--推挽输出
    GPIO_InitStructure.Pins = Usart1_Tx_Pin;  
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  // 模式--浮空输入
    GPIO_InitStructure.Pins = Usart1_Rx_Pin;   
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);  

    /*串口初始化*/
    CW_UART1->CR1_f.SYNC = 0;   // 0-异步全双工通信模式; 1-同步半双工通信模式
    CW_UART1->ICR = 0x00;   // 清除所有串口中断标志
    USART_InitStructure.USART_BaudRate = 9600;  // 波特率
    USART_InitStructure.USART_Over = USART_Over_16;	// 采样方式---16倍采样
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//禁止硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式  
    // 下面3条是CW特有的    
    USART_InitStructure.USART_UclkFreq = 64000000;  // 传输时钟UCLK频率 Hz
    USART_InitStructure.USART_StartBit = USART_StartBit_FE; // 起始位判定方式---下降沿(低功耗则选择低电平)
    USART_InitStructure.USART_Source = USART_Source_PCLK;   // 传输时钟源UCLK---PCLK(看时钟树)
    USART_Init(CW_UART1,&USART_InitStructure);  // 串口初始化
    
    /*NVIC初始化(不需要定义结构体)*/
    NVIC_SetPriority(UART1_IRQn,0); // 优先级，无优先级分组
    NVIC_EnableIRQ(UART1_IRQn); // 使能中断---中断通道号

    USART_ITConfig(CW_UART1,USART_IT_RC,ENABLE);    // 接收完成中断使能
}

/*
* @function: USART1_Send_Array
* @param: p_Arr -> 待发送数组 LEN -> 数组长度(使用sizeof计算)
* @retval: None
* @brief: 发送数组
*/
static void USART1_Send_Array(uint8_t* p_Arr, uint16_t LEN)
{
    uint16_t i;

    for (i = 0; i < LEN; i++)
    {
        USART_SendData_8bit(CW_UART1,*(p_Arr + i));
        // 等待发送完成，1：完成，0：还没完成
        while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);
    }
    while(USART_GetFlagStatus(CW_UART1, USART_FLAG_TXBUSY) == SET); // 等待串口空闲
}

/*
* @function: USART1_Send_String
* @param: p_Str -> 待发送字符串
* @retval: None
* @brief: 发送字符串
*/
static void USART1_Send_String(uint8_t* p_Str)
{
    while (*p_Str)
    {
        USART_SendData_8bit(CW_UART1,*p_Str);
        // 等待发送完成，1：完成，0：还没完成
        while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == RESET);
        p_Str++;
    }
    while(USART_GetFlagStatus(CW_UART1, USART_FLAG_TXBUSY) == SET); // 等待串口空闲
}
