/***************************************************************************
 * File: gtim.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: gtim.c
 -----------------------------------
注意：
    PWM普通输出
                GTIM1: PB4 --- CH1  PB5 --- CH2 
    PWM输入捕获(异常)
                GTIM2: PA0 --- CH1  PA1 --- CH2 
    PWM互补输出
                GTIM3: PC14 --- TOGN  PC15 --- TOGP                               
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Gtim1_PWM_Output_Init(uint16_t arr, uint16_t psc);   // GTIM1PWM输出初始化
static void Gtim1_Calculate(void); // GTIM1PWM计算
static void Gtim2_PWM_IC_Init(void);    // GTIM2输入捕获初始化
static void Gtim3_PWM_Toggle_Init(uint16_t, uint16_t);    // GTIM3互补输出初始化

/*====================================static function declaration area   END====================================*/
Gtim_t Gtim = 
{
    0,
    0,
    0,
    {0.0, 0.0},
    {200, 200},
    Gtim1_PWM_Output_Init,
    Gtim1_Calculate,
    Gtim2_PWM_IC_Init,
    Gtim3_PWM_Toggle_Init,
};


/*
* @function: Gtim1_PWM_Output_Init
* @param: arr -> 重装载值 psc -> 预分频(比如 GTIM_PRESCALER_DIV64)
* @retval: None
* @brief: GTIM1PWM输出初始化
*/
static void Gtim1_PWM_Output_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GTIM_InitTypeDef GTIM_InitStructure;

    // 打开时钟
    __RCC_GTIM1_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();

    PB04_AFx_GTIM1CH1();    // PB4 GTIM1-CH1
    PB05_AFx_GTIM1CH2();    // PB5 GTIM1-CH2

    GPIO_InitStructure.IT = GPIO_IT_NONE; // 管脚中断模式--无
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // 模式--推挽输出
    GPIO_InitStructure.Pins = GPIO_PIN_4 | GPIO_PIN_5;  
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure); 

    /*定时器配置*/
    GTIM_InitStructure.Mode = GTIM_MODE_TIME; // 定时器模式 --- 定时器模式
    GTIM_InitStructure.OneShotMode = GTIM_COUNT_CONTINUE;  // 单次/连续计数模式 --- 连续
    GTIM_InitStructure.Prescaler = psc;    // 预分频系数 --- psc
    GTIM_InitStructure.ReloadValue = arr;  // 重载值 --- arr
    GTIM_InitStructure.ToggleOutState = DISABLE;   // 翻转输出使能选择 --- 不使能
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStructure);   // 初始化
    /*通道1*/
    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);  // 比较输出功能初始化 --- 定时器 通道 有效电平极性-高电平
    /*通道2*/
    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL2, GTIM_OC_OUTPUT_PWM_HIGH);  // 比较输出功能初始化 --- 定时器 通道 有效电平极性-高电平
    /*设置初始占空比*/
    CW_GTIM1->CCR1 = Gtim.Gtim1_CH_Set_Pulse[0];   // 设置通道1初始占空比 --- 初始化默认50%占空比  
    CW_GTIM1->CCR2 = Gtim.Gtim1_CH_Set_Pulse[1];   // 设置通道2初始占空比 --- 初始化默认50%占空比   

    GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);    // 中断打开
    GTIM_Cmd(CW_GTIM1, ENABLE); // 使能定时器
    // 使能中断
    __disable_irq();
    NVIC_EnableIRQ(GTIM1_IRQn);
    __enable_irq();      
}

/*
* @function: Gtim1_Calculate
* @param: None
* @retval: None
* @brief: GTIM1PWM计算
*/
static __inline void Gtim1_Calculate(void)
{
    Gtim.Gtim1_CH_Fre = 1000000 / (CW_GTIM1->ARR); // 
    Gtim.Gtim1_CH_Duty[0] = (float)(CW_GTIM1->CCR1) / (CW_GTIM1->ARR) * 100.0f; // CH1
    Gtim.Gtim1_CH_Duty[1] = (float)(CW_GTIM1->CCR2) / (CW_GTIM1->ARR) * 100.0f; // CH2
    // 
    sprintf((char *)Page4.OLED096_Display_Buff[0], "G1T_Fre:%dHz", Gtim.Gtim1_CH_Fre);   // 
    OLED096.padString((char *)Page4.OLED096_Display_Buff[0], 16);                                                 
    sprintf((char *)Page4.OLED096_Display_Buff[1], "CH1_Duty:%.2f%%", Gtim.Gtim1_CH_Duty[0]); // 
    OLED096.padString((char *)Page4.OLED096_Display_Buff[1], 16);  
    sprintf((char *)Page4.OLED096_Display_Buff[2], "CH2_Duty:%.2f%%", Gtim.Gtim1_CH_Duty[1]); // 
    OLED096.padString((char *)Page4.OLED096_Display_Buff[2], 16);//    
}

/*
* @function: Gtim2_PWM_IC_Init
* @param: None
* @retval: None
* @brief: GTIM2输入捕获初始化
*/
static void Gtim2_PWM_IC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GTIM_InitTypeDef GTIM_InitStrucure;
    GTIM_ICInitTypeDef GTIM_ICInitStructure;   

    __RCC_GTIM2_CLK_ENABLE();   // 
    __RCC_GPIOA_CLK_ENABLE(); 
    /*GPIO*/
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pins = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    PA00_AFx_GTIM2CH1();    // AF
    PA01_AFx_GTIM2CH2();    

    __disable_irq();
    NVIC_EnableIRQ(GTIM2_IRQn);
    __enable_irq(); 
    
    GTIM_InitStrucure.Mode = GTIM_MODE_TIME;    
    GTIM_InitStrucure.OneShotMode = GTIM_COUNT_CONTINUE;    
    GTIM_InitStrucure.Prescaler = GTIM_PRESCALER_DIV8;  
    GTIM_InitStrucure.ReloadValue = 0xFFFF; // ARR
    GTIM_InitStrucure.ToggleOutState = DISABLE;
    GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStrucure);

    GTIM_ICInitStructure.CHx = GTIM_CHANNEL1;   // CH1
    GTIM_ICInitStructure.ICFilter = GTIM_CHx_FILTER_NONE;   // 过滤器 -- 无
    GTIM_ICInitStructure.ICInvert = GTIM_CHx_INVERT_ON; // 翻转 --- 无
    GTIM_ICInitStructure.ICPolarity = GTIM_ICPolarity_Rising;   // 上升沿
    GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStructure);

    GTIM_ICInitStructure.CHx = GTIM_CHANNEL2;   // CH2
    GTIM_ICInitStructure.ICPolarity = GTIM_ICPolarity_Falling;   // 下降沿
    GTIM_ICInit(CW_GTIM2, &GTIM_ICInitStructure);

    GTIM_ITConfig(CW_GTIM2, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_OV, ENABLE);
    GTIM_Cmd(CW_GTIM2, ENABLE);   
}

/*
* @function: Gtim3_PWM_Toggle_Init
* @param: arr -> 重装载值 psc ->  预分频(比如 GTIM_PRESCALER_DIV1024)
* @retval: None
* @brief: GTIM3互补输出初始化
*/
static void Gtim3_PWM_Toggle_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GTIM_InitTypeDef GTIM_InitStrucure; 

    __RCC_GTIM3_CLK_ENABLE();   // 
    __RCC_GPIOC_CLK_ENABLE();

    PC14_AFx_GTIM3TOGN();    // AF
    PC15_AFx_GTIM3TOGP(); 
    /*GPIO*/
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pins = GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
    
    GTIM_InitStrucure.Mode = GTIM_MODE_TIME;    
    GTIM_InitStrucure.OneShotMode = GTIM_COUNT_CONTINUE;    
    GTIM_InitStrucure.Prescaler = psc;  
    GTIM_InitStrucure.ReloadValue = arr; // ARR
    GTIM_InitStrucure.ToggleOutState = ENABLE;
    GTIM_TimeBaseInit(CW_GTIM3, &GTIM_InitStrucure); 
    GTIM_Cmd(CW_GTIM3, ENABLE);
}

