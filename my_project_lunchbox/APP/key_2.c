/***************************************************************************
 * File: key_2.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 最小系统板的按键---外部中断方式(按键高电平有效)
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Key_2_Init(void);   // 按键初始化

/*====================================static function declaration area   END====================================*/


Key_2_t Key_2 = 
{
    Key_2_Init,
};

/*
* @function: Key_2_Init
* @param: None
* @retval: None
* @brief: 按键初始化
*/
static void Key_2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能时钟
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_RISING;          // 管脚中断模式--上升沿触发
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLDOWN; // 模式--下拉输入
    GPIO_InitStructure.Pins = SW1_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure); 

    GPIOB_INTFLAG_CLR(bv2);  // 清除PB2中断标志
    NVIC_EnableIRQ(GPIOB_IRQn); // 使能NVIC 
}
