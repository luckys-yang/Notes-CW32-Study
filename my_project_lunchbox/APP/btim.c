/***************************************************************************
 * File: btim.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 基本定时器
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void BTIM1_Init(uint16_t, uint16_t); // BTIM1初始化

/*====================================static function declaration area   END====================================*/
Btim_t Btim = 
{
    BTIM1_Init,
};

/*
* @function: BTIM1_Init
* @param: arr -> 重装载值 psc -> 时钟分频系数(例如BTIM_PRS_DIV1)
* @retval: None
* @brief: BTIM1初始化
*/
static void BTIM1_Init(uint16_t arr, uint16_t psc)
{
    BTIM_TimeBaseInitTypeDef BTM_TimerBaseInitStructure;

    __RCC_BTIM_CLK_ENABLE();    // 打开定时器时钟

    __disable_irq();    // 关闭中断
    NVIC_EnableIRQ(BTIM1_IRQn); // 使能BTIM1中断
    __enable_irq(); // 打开中断

    BTM_TimerBaseInitStructure.BTIM_Mode = BTIM_Mode_TIMER; // 模式---定时器模式
    BTM_TimerBaseInitStructure.BTIM_OPMode = BTIM_OPMode_Repetitive;    // 连续模式
    BTM_TimerBaseInitStructure.BTIM_Period = arr;   // 重装载值
    BTM_TimerBaseInitStructure.BTIM_Prescaler = psc;    // 时钟预分频系数
    BTIM_TimeBaseInit(CW_BTIM1, &BTM_TimerBaseInitStructure);			//配置BTIM1定时器

	BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);					//使能BTIM1中断
	BTIM_Cmd(CW_BTIM1, ENABLE);										//使能定时器
}

