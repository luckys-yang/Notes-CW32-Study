/***************************************************************************
 * File: atim.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: �߼���ʱ��
 -----------------------------------
ע�⣺
    PWM���벶��(�쳣)
                ATIM: PB2 --- CH1A
    PWM����Ƚ�
                GTIM3: PC14 --- TOGN  PC15 --- TOGP                               
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Atim_Base_Init(uint16_t arr, uint16_t psc); // ATIM������ʱ��ʼ��
static void Atim_PWM_Input_Init(void);  // ���벶���ʼ��
static void Atim_PWM_Output_OC_Init(void);  // �������PWM�źų�ʼ��
/*====================================static function declaration area   END====================================*/

Atim_t Atim = 
{
    Atim_Base_Init,
    Atim_PWM_Input_Init,
    Atim_PWM_Output_OC_Init,
};

/*
* @function: Atim_Base_Init
* @param: arr -> ��װ��ֵ psc -> ʱ�ӷ�Ƶϵ��(���� ATIM_Prescaler_DIV1)
* @retval: None
* @brief: ATIM������ʱ��ʼ��
*/
static void Atim_Base_Init(uint16_t arr, uint16_t psc)
{
    ATIM_InitTypeDef ATIM_InitStructure;

    __RCC_ATIM_CLK_ENABLE();    // ��ʱ��

    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);  // ���ж�
    __enable_irq();       

    ATIM_InitStructure.BufferState = ENABLE;               // ARR����ʹ�� --- ʹ�ܻ���Ĵ���
    ATIM_InitStructure.ClockSelect = ATIM_CLOCK_PCLK;      // ����ʱ��ѡ�� --- ѡ��PCLKʱ�Ӽ���
    ATIM_InitStructure.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;    // ��������ģʽ --- ���ض���
    ATIM_InitStructure.CounterDirection = ATIM_COUNTING_UP;        // ��������ģʽ --- ���ϼ���
    ATIM_InitStructure.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    // ����ģʽ --- ��������ģʽ
    ATIM_InitStructure.OverFlowMask = DISABLE;        // ���������ʹ�� --- �ظ������������������
    ATIM_InitStructure.Prescaler = psc;    // Ԥ��Ƶϵ��
    ATIM_InitStructure.ReloadValue = arr;         //  ARR��ֵ��ȡֵ��Χ0x0000~0xFFFF
    ATIM_InitStructure.RepetitionCounter = 0;      // �ظ����ڼ���ֵ��RCR�Ĵ���
    ATIM_InitStructure.UnderFlowMask = DISABLE;   // ���������ʹ�� --- �ظ����������������

    ATIM_Init(&ATIM_InitStructure);
    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);             // ATIM�ж� --- ���ظ�������������������ж�
    ATIM_Cmd(ENABLE);  // ����      
}

/*
* @function: Atim_PWM_Input_Init
* @param: None
* @retval: None
* @brief: ���벶���ʼ��
*/
static void Atim_PWM_Input_Init(void)
{
    ATIM_InitTypeDef ATIM_InitStructure;
    ATIM_ICInitTypeDef ATIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    __RCC_GPIOB_CLK_ENABLE();   
    __RCC_ATIM_CLK_ENABLE();
    /*gpio init*/
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    PB02_AFx_ATIMCH1A();    // PB2 --- CH1A
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    /*open nvic*/ 
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();

    ATIM_InitStructure.BufferState = DISABLE;
    ATIM_InitStructure.ClockSelect = ATIM_CLOCK_PCLK;
    ATIM_InitStructure.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
    ATIM_InitStructure.CounterDirection = ATIM_COUNTING_UP;
    ATIM_InitStructure.CounterOPMode = ATIM_OP_MODE_REPETITIVE;
    ATIM_InitStructure.OverFlowMask = DISABLE;
    ATIM_InitStructure.Prescaler = ATIM_Prescaler_DIV64;
    ATIM_InitStructure.ReloadValue = 0xFFFF;
    ATIM_InitStructure.RepetitionCounter = 0;
    ATIM_InitStructure.UnderFlowMask = DISABLE;  
    ATIM_Init(&ATIM_InitStructure);

    ATIM_ICInitStructure.ICFilter = ATIM_ICFILTER_NONE; // �����˲�����
    ATIM_ICInitStructure.ICPolarity = ATIM_ICPOLARITY_BOTHEDGE; // ���벶���ԣ��������½���˫��
    ATIM_IC1AInit(&ATIM_ICInitStructure);   // ���벶��ͨ��1A����

    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);  // ATIM�ж�����
    ATIM_CH1Config(ATIM_CHxA_CIE, ENABLE);  // ����ͨ��1�Ĺ���
    ATIM_Cmd(ENABLE);             
}

/*
* @function: Atim_PWM_Output_OC_Init
* @param: None
* @retval: None
* @brief: ����Ƚϳ�ʼ��
*/
static void Atim_PWM_Output_OC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ATIM_InitTypeDef ATIM_InitStructure;
    ATIM_OCInitTypeDef ATIM_OCInitStructure;

    __RCC_ATIM_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();
    /*gpio init*/
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pins = GPIO_PIN_2 | GPIO_PIN_13;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    PB02_AFx_ATIMCH1A();
    PB13_AFx_ATIMCH1B();
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);   
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();    

    ATIM_InitStructure.BufferState = DISABLE;
    ATIM_InitStructure.ClockSelect = ATIM_CLOCK_PCLK;
    ATIM_InitStructure.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
    ATIM_InitStructure.CounterDirection = ATIM_COUNTING_UP;
    ATIM_InitStructure.CounterOPMode = ATIM_OP_MODE_REPETITIVE;
    ATIM_InitStructure.OverFlowMask = DISABLE;
    ATIM_InitStructure.Prescaler = ATIM_Prescaler_DIV1;
    ATIM_InitStructure.ReloadValue = 6400;   // 10KHz            
    ATIM_InitStructure.RepetitionCounter = 0;
    ATIM_InitStructure.UnderFlowMask = DISABLE;
    ATIM_Init(&ATIM_InitStructure);  

    ATIM_OCInitStructure.BufferState = DISABLE;     // �Ƚϻ���ʹ��״̬
    ATIM_OCInitStructure.OCDMAState = DISABLE;  // �Ƚ�ƥ�䴥��DMAʹ��״̬
    ATIM_OCInitStructure.OCInterruptSelect = ATIM_OC_IT_UP_COUNTER; // �Ƚ�ƥ�䴥���ж�ʹ��״̬
    ATIM_OCInitStructure.OCInterruptState = ENABLE; // �Ƚ�ƥ�䴥���ж�ʹ��״̬
    ATIM_OCInitStructure.OCMode = ATIM_OCMODE_PWM1; //  �Ƚ�ģʽ����
    ATIM_OCInitStructure.OCPolarity = ATIM_OCPOLARITY_NONINVERT;    // �˿ڼ���ѡ�����򡢷���---����ת��ƽ
    ATIM_OC1AInit(&ATIM_OCInitStructure);   // CH1A�Ƚ��������
    ATIM_OC1BInit(&ATIM_OCInitStructure);   // CH1B�Ƚ��������  

    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);  // ATIM�ж�����
    ATIM_CH1Config(ATIM_CHxB_CIE | ATIM_CHxA_CIE, ENABLE);  // ����ͨ��1�Ĺ���
    ATIM_SetCompare1A(3200); // PB2 50%ռ�ձ�
    ATIM_SetCompare1B(2560);    // PB13 40%ռ�ձ�
    // ATIM_PWMOutputConfig(OCREFA_TYPE_SINGLE, OUTPUT_TYPE_COMP, 1);  // ����PWM����Ĳ���
    ATIM_Cmd(ENABLE);   // ATIM ����   
    ATIM_CtrlPWMOutputs(ENABLE);    // ʹ��PWM���
}

