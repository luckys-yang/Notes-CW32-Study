/***************************************************************************
 * File: adc1.c
 * Author: Luckys.
 * Date: 2023/06/15
 * description: ADC
 -----------------------------------
注意：板子的电位器，对应ADC通道引脚是PB0，然后还添加了另外3个通道分别是PA4 PA5 PA6 
注意：需要在【public.h】进行ADC选择模式(单通道单次/序列扫描)
接线：
    光敏电阻：
            GND --- GND
            3.3V --- 3.3V
            S --- ADC通道PA5
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/
static void ADC1_Single_Channel_One_Init(void);    // ADC单通道单次采集初始化
static void ADC1_Serial_Scan_Init(void);    // ADC序列扫描采集初始化
static void ADC1_Single_Channel_One_Convert(void); // ADC单通道单次采集转换
static void ADC1_Serial_Scan_Convert(void);    // ADC序列扫描采集转换

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
* @brief: ADC单通道单次采集初始化
*/
static void ADC1_Single_Channel_One_Init(void)
{
    ADC_SingleChTypeDef ADC_SingleChInitStructure;

    // 打开时钟
    __RCC_GPIOA_CLK_ENABLE();   
    __RCC_ADC_CLK_ENABLE();
    
    // 引脚设为模拟输入
    PA00_ANALOG_ENABLE();   // PA0 --- ADC_CH0

    /*ADC配置*/
    ADC_SingleChInitStructure.ADC_Chmux = ADC_ExInputCH0;   // 输入通道 -- 0
    ADC_SingleChInitStructure.ADC_DiscardEn = ADC_DiscardNull;  // 单通道ADC转换结果保存策略配置
    ADC_SingleChInitStructure.ADC_WdtStruct.ADC_WdtAll = ADC_WdtDisable;    // ADC模拟看门狗使能 -- 不使能
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_AccEn = ADC_AccDisable;   // 转换结果累加是否使能 -- 否
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_Align = ADC_AlignRight; // 转换结果对齐方式 -- 右对齐(采集多少就是多少)
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div16;    // 时钟选择(ADC工作时钟ADCCLK，由系统时钟PCLK 经预分频器分频得到) -- 4分频
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_DMAEn = ADC_DmaDisable; // ADC转换完成是/否触发DMA使能 -- 不使能
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_InBufEn = ADC_BufEnable;   // ADC输入增益使能 -- 开启
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;    // 操作模式 -- 单通道单次转换模式
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_SampleTime = ADC_SampTime10Clk;    // ADC采样时间 -- 10个ADCCLK 个数
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_TsEn = ADC_TsDisable;  // 内置温度传感器是/否使能 -- 否
    ADC_SingleChInitStructure.ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;   // ADC参考电压 -- 使用芯片供电电压

    ADC_SingleChOneModeCfg(&ADC_SingleChInitStructure); // ADC单通道单次转换模式配置
    ADC_Enable();   // ADC使能
    ADC_SoftwareStartConvCmd(ENABLE);   // ADC转换软件启动
}

/*
* @function: ADC1_Serial_Scan_Init
* @param: None
* @retval: None
* @brief: ADC序列扫描采集初始化
*/
static void ADC1_Serial_Scan_Init(void)
{
    ADC_SerialChTypeDef ADC_SerialChInitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    // 打开时钟
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();   
    __RCC_ADC_CLK_ENABLE();
    
    // 引脚设为模拟输入
    PA04_ANALOG_ENABLE();   // PA4 --- ADC_CH4
    PA05_ANALOG_ENABLE();   // PA5 --- ADC_CH5
    PA06_ANALOG_ENABLE();   // PA6 --- ADC_CH6
    PB00_ANALOG_ENABLE();   // PB0 --- ADC_CH8

    ADC_StructInit(&ADC_InitStructure); // 默认值初始化
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div16;   // ADC CLK: 64000000/16 = 4000KHz 不能超过24M
    ADC_SerialChInitStructure.ADC_SqrEns = ADC_SqrEns03;    // 采集4个通道 
    ADC_SerialChInitStructure.ADC_Sqr0Chmux = ADC_SqrCh8;
    ADC_SerialChInitStructure.ADC_Sqr1Chmux = ADC_SqrCh4;
    ADC_SerialChInitStructure.ADC_Sqr2Chmux = ADC_SqrCh5;
    ADC_SerialChInitStructure.ADC_Sqr3Chmux = ADC_SqrCh6;
    ADC_SerialChInitStructure.ADC_InitStruct = ADC_InitStructure;

    ADC_SerialChScanModeCfg(&ADC_SerialChInitStructure);    // 序列扫描转换初始化
    ADC_ITConfig(ADC_IT_EOS, ENABLE);   // 中断使能
    ADC_EnableIrq(ADC_INT_PRIORITY);    // 优先级
    ADC_ClearITPendingAll();    // 清除所有ADC中断标志
    ADC_Enable();   // ADC使能
    ADC_SoftwareStartConvCmd(ENABLE);   // ADC转换软件启动
}

/*
* @function: ADC1_Single_Channel_One_Convert
* @param: None
* @retval: None
* @brief: ADC单通道单次采集转换
*/
static void ADC1_Single_Channel_One_Convert(void)
{
    uint16_t adc_temp;

    ADC_SoftwareStartConvCmd(ENABLE);

    while(ADC_GetITStatus(ADC_IT_EOC))
    {
        ADC_ClearITPendingBit(ADC_IT_EOC);  // 清除标志位
        adc_temp = ADC_GetConversionValue();    // 获取单次转换的值
        ADC_1.ADC_Single_Result = (float)adc_temp*(3.3f/4096u);  // 计算电压结果
    }
}

/*
* @function: ADC1_Serial_Scan_Convert
* @param: None
* @retval: None
* @brief: ADC序列扫描采集转换
*/
static void ADC1_Serial_Scan_Convert(void)
{
    uint16_t adc_temp[4];

    while (!(ADC_1.gFlagIrq & ADC_ISR_EOS_Msk));    // &上掩码0x2
    ADC_1.gFlagIrq = 0u;    // 置0
    ADC_GetSqr0Result(adc_temp);                                        // 获取序列0通道转换的值
    ADC_GetSqr1Result(&adc_temp[1]);                                    // 获取序列1通道转换的值
    ADC_GetSqr2Result(&adc_temp[2]);                                    // 获取序列2通道转换的值
    ADC_GetSqr3Result(&adc_temp[3]);                                    // 获取序列3通道转换的值
    ADC_SoftwareStartConvCmd(ENABLE);                                   // ADC转换软件启动
    ADC_1.ADC_Serial_Result_Arr[0] = (float)adc_temp[0] * (3.3f / 4096u); // 计算电压结果--PB0
    ADC_1.ADC_Serial_Result_Arr[1] = (float)adc_temp[1] * (3.3f / 4096u); // 计算电压结果--PA4
    ADC_1.ADC_Serial_Result_Arr[2] = (float)adc_temp[2] * (3.3f / 4096u); // 计算电压结果--PA5
    ADC_1.ADC_Serial_Result_Arr[3] = (float)adc_temp[3] * (3.3f / 4096u); // 计算电压结果--PA6
}
