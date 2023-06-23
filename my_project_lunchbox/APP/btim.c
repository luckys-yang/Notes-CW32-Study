/***************************************************************************
 * File: btim.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: ������ʱ��
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void BTIM1_Init(uint16_t, uint16_t); // BTIM1��ʼ��

/*====================================static function declaration area   END====================================*/
Btim_t Btim = 
{
    BTIM1_Init,
};

/*
* @function: BTIM1_Init
* @param: arr -> ��װ��ֵ psc -> ʱ�ӷ�Ƶϵ��(����BTIM_PRS_DIV1)
* @retval: None
* @brief: BTIM1��ʼ��
*/
static void BTIM1_Init(uint16_t arr, uint16_t psc)
{
    BTIM_TimeBaseInitTypeDef BTM_TimerBaseInitStructure;

    __RCC_BTIM_CLK_ENABLE();    // �򿪶�ʱ��ʱ��

    __disable_irq();    // �ر��ж�
    NVIC_EnableIRQ(BTIM1_IRQn); // ʹ��BTIM1�ж�
    __enable_irq(); // ���ж�

    BTM_TimerBaseInitStructure.BTIM_Mode = BTIM_Mode_TIMER; // ģʽ---��ʱ��ģʽ
    BTM_TimerBaseInitStructure.BTIM_OPMode = BTIM_OPMode_Repetitive;    // ����ģʽ
    BTM_TimerBaseInitStructure.BTIM_Period = arr;   // ��װ��ֵ
    BTM_TimerBaseInitStructure.BTIM_Prescaler = psc;    // ʱ��Ԥ��Ƶϵ��
    BTIM_TimeBaseInit(CW_BTIM1, &BTM_TimerBaseInitStructure);			//����BTIM1��ʱ��

	BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);					//ʹ��BTIM1�ж�
	BTIM_Cmd(CW_BTIM1, ENABLE);										//ʹ�ܶ�ʱ��
}

