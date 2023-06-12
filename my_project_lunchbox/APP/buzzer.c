/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/12
 * description: 蜂鸣器
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Buzzer_Init(void);  // 蜂鸣器初始化
static void Buzzer_ON(void);    // 蜂鸣器打开
static void Buzzer_OFF(void);   // 蜂鸣器关闭

/*====================================static function declaration area   END====================================*/
Buzzer_t Buzzer = 
{
    Buzzer_Status_OFF,
    Buzzer_Init,
    Buzzer_ON,
    Buzzer_OFF,
};

/*
* @function: Buzzer_Init
* @param: None
* @retval: None
* @brief: 蜂鸣器初始化
*/
static void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能时钟
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;          // 管脚中断模式--无
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 模式--推挽输出
    GPIO_InitStructure.Pins = Buzzer_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    GPIO_WritePin(CW_GPIOB,Buzzer_Pin,GPIO_Pin_RESET); // 默认蜂鸣器关闭
}

/*
* @function: Buzzer_ON
* @param: None
* @retval: None
* @brief: 蜂鸣器打开
*/
static void Buzzer_ON(void)
{
    GPIO_WritePin(CW_GPIOB,Buzzer_Pin,GPIO_Pin_SET);
    Buzzer.Buzzer_Status = Buzzer_Status_ON;
}

/*
* @function: Buzzer_OFF
* @param: None
* @retval: None
* @brief: 蜂鸣器关闭
*/
static void Buzzer_OFF(void)
{
    GPIO_WritePin(CW_GPIOB,Buzzer_Pin,GPIO_Pin_RESET);
    Buzzer.Buzzer_Status = Buzzer_Status_OFF;
}