/***************************************************************************
 * File: adc1.c
 * Author: Luckys.
 * Date: 2023/06/15
 * description: ADC
 -----------------------------------
ע�⣺���ӵĵ�λ������ӦADCͨ��������PB0��Ȼ�����������3��ͨ���ֱ���PA4 PA5 PA6 
ע�⣺��Ҫ�ڡ�public.h������ADCѡ��ģʽ(��ͨ������/����ɨ��)
���ߣ�
    �������裺
            GND --- GND
            3.3V --- 3.3V
            S --- ADCͨ��PA5
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/
static void ADC1_Single_Channel_One_Init(void);    // ADC��ͨ�����βɼ���ʼ��
static void ADC1_Serial_Scan_Init(void);    // ADC����ɨ��ɼ���ʼ��
static void ADC1_Single_Channel_One_Convert(void); // ADC��ͨ�����βɼ�ת��
static void ADC1_Serial_Scan_Convert(void);    // ADC����ɨ��ɼ�ת��

/*====================================static function declaration area   END====================================*/
ADC_1_t ADC_1 = 
{
    0,
    0,
    {0.0},
    ADC1_Single_Channel_One_Init,
    ADC1_Serial_Scan_Init,
    ADC1_Single_Channel_One_Convert,
    ADC1_Serial_Scan_Convert,
};

/*
* @function: ADC1_Single_Channel_One_Init
* @param: None
* @retval: None
* @brief: ADC��ͨ�����βɼ���ʼ��
*/
static void ADC1_Single_Channel_One_Init(void)
{
    ADC_SingleChTypeDef ADC_SingleChInitStructure;

    // ��ʱ��
    __RCC_GPIOA_CLK_ENABLE();   
    __RCC_ADC_CLK_ENABLE();
    
    // ������Ϊģ������
    PA00_ANALOG_ENABLE();   // PA0 --- ADC_CH0

    /*ADC����*/
    ADC_SingleChInitStructure.ADC_Chmux = ADC_ExInputCH0;   // ����ͨ�� -- 0
    ADC_SingleChInitStructure.ADC_DiscardEn = ADC_DiscardNull;  // ��ͨ��ADCת����������������
    ADC_SingleChInitStructure.ADC_WdtStruct.ADC_WdtAll = ADC_WdtDisable;    // ADCģ�⿴�Ź�ʹ�� -- ��ʹ��
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_AccEn = ADC_AccDisable;   // ת������ۼ��Ƿ�ʹ�� -- ��
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_Align = ADC_AlignRight; // ת��������뷽ʽ -- �Ҷ���(�ɼ����پ��Ƕ���)
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div16;    // ʱ��ѡ��(ADC����ʱ��ADCCLK����ϵͳʱ��PCLK ��Ԥ��Ƶ����Ƶ�õ�) -- 4��Ƶ
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_DMAEn = ADC_DmaDisable; // ADCת�������/�񴥷�DMAʹ�� -- ��ʹ��
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_InBufEn = ADC_BufEnable;   // ADC��������ʹ�� -- ����
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;    // ����ģʽ -- ��ͨ������ת��ģʽ
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_SampleTime = ADC_SampTime10Clk;    // ADC����ʱ�� -- 10��ADCCLK ����
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_TsEn = ADC_TsDisable;  // �����¶ȴ�������/��ʹ�� -- ��
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;   // ADC�ο���ѹ -- ʹ��оƬ�����ѹ

    ADC_SingleChOneModeCfg(&ADC_SingleChInitStructure); // ADC��ͨ������ת��ģʽ����
    ADC_Enable();   // ADCʹ��
    ADC_SoftwareStartConvCmd(ENABLE);   // ADCת���������
}

/*
* @function: ADC1_Serial_Scan_Init
* @param: None
* @retval: None
* @brief: ADC����ɨ��ɼ���ʼ��
*/
static void ADC1_Serial_Scan_Init(void)
{
    ADC_SerialChTypeDef ADC_SerialChInitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    // ��ʱ��
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();   
    __RCC_ADC_CLK_ENABLE();
    
    // ������Ϊģ������
    PA04_ANALOG_ENABLE();   // PA4 --- ADC_CH4
    PA05_ANALOG_ENABLE();   // PA5 --- ADC_CH5
    PA06_ANALOG_ENABLE();   // PA6 --- ADC_CH6
    PB00_ANALOG_ENABLE();   // PB0 --- ADC_CH8

    ADC_StructInit(&ADC_InitStructure); // Ĭ��ֵ��ʼ��
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div16;   // ADC CLK: 64000000/16 = 4000KHz ���ܳ���24M
    ADC_SerialChInitStructure.ADC_SqrEns = ADC_SqrEns03;    // �ɼ�4��ͨ�� 
    ADC_SerialChInitStructure.ADC_Sqr0Chmux = ADC_SqrCh8;
    ADC_SerialChInitStructure.ADC_Sqr1Chmux = ADC_SqrCh4;
    ADC_SerialChInitStructure.ADC_Sqr2Chmux = ADC_SqrCh5;
    ADC_SerialChInitStructure.ADC_Sqr3Chmux = ADC_SqrCh6;
    ADC_SerialChInitStructure.ADC_InitStruct = ADC_InitStructure;

    ADC_SerialChScanModeCfg(&ADC_SerialChInitStructure);    // ����ɨ��ת����ʼ��
    ADC_ITConfig(ADC_IT_EOS, ENABLE);   // �ж�ʹ��
    ADC_EnableIrq(ADC_INT_PRIORITY);    // ���ȼ�
    ADC_ClearITPendingAll();    // �������ADC�жϱ�־
    ADC_Enable();   // ADCʹ��
    ADC_SoftwareStartConvCmd(ENABLE);   // ADCת���������
}

/*
* @function: ADC1_Single_Channel_One_Convert
* @param: None
* @retval: None
* @brief: ADC��ͨ�����βɼ�ת��
*/
static void ADC1_Single_Channel_One_Convert(void)
{
    uint16_t adc_temp;

    ADC_SoftwareStartConvCmd(ENABLE);

    while(ADC_GetITStatus(ADC_IT_EOC))
    {
        ADC_ClearITPendingBit(ADC_IT_EOC);  // �����־λ
        adc_temp = ADC_GetConversionValue();    // ��ȡ����ת����ֵ
        ADC_1.ADC_Single_Result = (float)adc_temp*(3.3f/4096u);  // �����ѹ���
    }
}

/*
* @function: ADC1_Serial_Scan_Convert
* @param: None
* @retval: None
* @brief: ADC����ɨ��ɼ�ת��
*/
static void ADC1_Serial_Scan_Convert(void)
{
    uint16_t adc_temp[4];

    while (!(ADC_1.gFlagIrq & ADC_ISR_EOS_Msk));    // &������0x2
    ADC_1.gFlagIrq = 0u;    // ��0
    ADC_GetSqr0Result(adc_temp);                                        // ��ȡ����0ͨ��ת����ֵ
    ADC_GetSqr1Result(&adc_temp[1]);                                    // ��ȡ����1ͨ��ת����ֵ
    ADC_GetSqr2Result(&adc_temp[2]);                                    // ��ȡ����2ͨ��ת����ֵ
    ADC_GetSqr3Result(&adc_temp[3]);                                    // ��ȡ����3ͨ��ת����ֵ
    ADC_SoftwareStartConvCmd(ENABLE);                                   // ADCת���������
    ADC_1.ADC_Serial_Result_Arr[0] = (float)adc_temp[0] * (3.3f / 4096u); // �����ѹ���--PB0
    ADC_1.ADC_Serial_Result_Arr[1] = (float)adc_temp[1] * (3.3f / 4096u); // �����ѹ���--PA4
    ADC_1.ADC_Serial_Result_Arr[2] = (float)adc_temp[2] * (3.3f / 4096u); // �����ѹ���--PA5
    ADC_1.ADC_Serial_Result_Arr[3] = (float)adc_temp[3] * (3.3f / 4096u); // �����ѹ���--PA6
}
