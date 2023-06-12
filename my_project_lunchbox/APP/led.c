/***************************************************************************
 * File: led.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: LED
****************************************************************************/

#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Led_Init(void);      // LED��ʼ��
static void Led_ON(Led_Num_t);   // ��
static void Led_OFF(Led_Num_t);  // �ر�
static void Led_Flip(Led_Num_t); // ��ת

/*====================================static function declaration area   END====================================*/

Led_t Led = 
{
    Led_Init,
    Led_ON,
    Led_OFF,
    Led_Flip,
};

/*
* @function: Led_Init
* @param: None
* @retval: None
* @brief: LED��ʼ��
*/
static void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ʹ��ʱ��
    __RCC_GPIOC_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE; // �ܽ��ж�ģʽ--��
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // ģʽ--�������
    GPIO_InitStructure.Pins = Led1_Pin | Led2_Pin;  
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = Led3_Pin;
    GPIO_Init(CW_GPIOC, &GPIO_InitStructure);

    GPIO_WritePin(CW_GPIOA, Led1_Pin | Led2_Pin, GPIO_Pin_SET); // Ĭ����
    GPIO_WritePin(CW_GPIOC, Led3_Pin, GPIO_Pin_SET);
}

/*
* @function: Led_ON
* @param: LEDx -> 1,2,3
* @retval: None
* @brief: LED��
*/
static void Led_ON(Led_Num_t LEDx)
{
    switch(LEDx)
    {
        case LED1:
        {
            GPIO_WritePin(CW_GPIOA, Led1_Pin, GPIO_Pin_RESET);
            break;
        }
        case LED2:
        {
            GPIO_WritePin(CW_GPIOA, Led2_Pin, GPIO_Pin_RESET);
            break;
        }
        case LED3:
        {
            GPIO_WritePin(CW_GPIOC, Led3_Pin, GPIO_Pin_RESET);
            break;
        }
        default:
        {
            GPIO_WritePin(CW_GPIOA, Led1_Pin, GPIO_Pin_RESET);
            break;
        }
    }
}

/*
* @function: Led_OFF
* @param: LEDx -> 1,2,3
* @retval: None
* @brief: LED�ر�
*/
static void Led_OFF(Led_Num_t LEDx)
{
    switch(LEDx)
    {
        case LED1:
        {
            GPIO_WritePin(CW_GPIOA, Led1_Pin, GPIO_Pin_SET);
            break;
        }
        case LED2:
        {
            GPIO_WritePin(CW_GPIOA, Led2_Pin, GPIO_Pin_SET);
            break;
        }
        case LED3:
        {
            GPIO_WritePin(CW_GPIOC, Led3_Pin, GPIO_Pin_SET);
            break;
        }
        default:
        {
            GPIO_WritePin(CW_GPIOA, Led1_Pin, GPIO_Pin_SET);
            break;
        }
    }    
}

/*
* @function: Led_Flip
* @param: LEDx -> 1,2,3
* @retval: None
* @brief: LED��ʼ��
*/
static void Led_Flip(Led_Num_t LEDx)
{
    switch(LEDx)
    {
        case LED1:
        {
            GPIO_TogglePin(CW_GPIOA, Led1_Pin);
            break;
        }
        case LED2:
        {
            GPIO_TogglePin(CW_GPIOA, Led2_Pin);
            break;
        }
        case LED3:
        {
            GPIO_TogglePin(CW_GPIOC, Led3_Pin);
            break;
        }
        default:
        {
            GPIO_TogglePin(CW_GPIOA, Led1_Pin);
            break;
        }
    }    
}
