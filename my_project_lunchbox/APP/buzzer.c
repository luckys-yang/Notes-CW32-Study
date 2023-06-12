/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/12
 * description: ������
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Buzzer_Init(void);  // ��������ʼ��
static void Buzzer_ON(void);    // ��������
static void Buzzer_OFF(void);   // �������ر�

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
* @brief: ��������ʼ��
*/
static void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ʹ��ʱ��
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;          // �ܽ��ж�ģʽ--��
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // ģʽ--�������
    GPIO_InitStructure.Pins = Buzzer_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    GPIO_WritePin(CW_GPIOB,Buzzer_Pin,GPIO_Pin_RESET); // Ĭ�Ϸ������ر�
}

/*
* @function: Buzzer_ON
* @param: None
* @retval: None
* @brief: ��������
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
* @brief: �������ر�
*/
static void Buzzer_OFF(void)
{
    GPIO_WritePin(CW_GPIOB,Buzzer_Pin,GPIO_Pin_RESET);
    Buzzer.Buzzer_Status = Buzzer_Status_OFF;
}