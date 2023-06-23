/***************************************************************************
 * File: atim.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 高级定时器
 -----------------------------------
注意：
    PWM输入捕获(异常)
                ATIM: PB2 --- CH1A
    PWM输出比较
                GTIM3: PC14 --- TOGN  PC15 --- TOGP                               
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Atim_Base_Init(uint16_t arr, uint16_t psc); // ATIM基本定时初始化
static void Atim_PWM_Input_Init(void);  // 输入捕获初始化
static void Atim_PWM_Output_OC_Init(void);  // 输出互补PWM信号初始化
/*====================================static function declaration area   END====================================*/

Atim_t Atim = 
{
    Atim_Base_Init,
    Atim_PWM_Input_Init,
    Atim_PWM_Output_OC_Init,
};

/*
* @function: Atim_Base_Init
* @param: arr -> 重装载值 psc -> 时钟分频系数(例如 ATIM_Prescaler_DIV1)
* @retval: None
* @brief: ATIM基本定时初始化
*/
static void Atim_Base_Init(uint16_t arr, uint16_t psc)
{
    ATIM_InitTypeDef ATIM_InitStructure;

    __RCC_ATIM_CLK_ENABLE();    // 打开时钟

    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);  // 打开中断
    __enable_irq();       

    ATIM_InitStructure.BufferState = ENABLE;               // ARR缓存使能 --- 使能缓存寄存器
    ATIM_InitStructure.ClockSelect = ATIM_CLOCK_PCLK;      // 计数时钟选择 --- 选择PCLK时钟计数
    ATIM_InitStructure.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;    // 计数对齐模式 --- 边沿对齐
    ATIM_InitStructure.CounterDirection = ATIM_COUNTING_UP;        // 计数对齐模式 --- 向上计数
    ATIM_InitStructure.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    // 运行模式 --- 连续运行模式
    ATIM_InitStructure.OverFlowMask = DISABLE;        // 上溢出屏蔽使能 --- 重复计数器上溢出不屏蔽
    ATIM_InitStructure.Prescaler = psc;    // 预分频系数
    ATIM_InitStructure.ReloadValue = arr;         //  ARR赋值，取值范围0x0000~0xFFFF
    ATIM_InitStructure.RepetitionCounter = 0;      // 重复周期计数值，RCR寄存器
    ATIM_InitStructure.UnderFlowMask = DISABLE;   // 下溢出屏蔽使能 --- 重复计数下溢出不屏蔽

    ATIM_Init(&ATIM_InitStructure);
    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);             // ATIM中断 --- 有重复计数器溢出产生进入中断
    ATIM_Cmd(ENABLE);  // 启动      
}

/*
* @function: Atim_PWM_Input_Init
* @param: None
* @retval: None
* @brief: 输入捕获初始化
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

    ATIM_ICInitStructure.ICFilter = ATIM_ICFILTER_NONE; // 输入滤波配置
    ATIM_ICInitStructure.ICPolarity = ATIM_ICPOLARITY_BOTHEDGE; // 输入捕获极性：上升、下降、双沿
    ATIM_IC1AInit(&ATIM_ICInitStructure);   // 输入捕获通道1A设置

    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);  // ATIM中断设置
    ATIM_CH1Config(ATIM_CHxA_CIE, ENABLE);  // 设置通道1的功能
    ATIM_Cmd(ENABLE);             
}

/*
* @function: Atim_PWM_Output_OC_Init
* @param: None
* @retval: None
* @brief: 输出比较初始化
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

    ATIM_OCInitStructure.BufferState = DISABLE;     // 比较缓存使能状态
    ATIM_OCInitStructure.OCDMAState = DISABLE;  // 比较匹配触发DMA使能状态
    ATIM_OCInitStructure.OCInterruptSelect = ATIM_OC_IT_UP_COUNTER; // 比较匹配触发中断使能状态
    ATIM_OCInitStructure.OCInterruptState = ENABLE; // 比较匹配触发中断使能状态
    ATIM_OCInitStructure.OCMode = ATIM_OCMODE_PWM1; //  比较模式配置
    ATIM_OCInitStructure.OCPolarity = ATIM_OCPOLARITY_NONINVERT;    // 端口极性选择：正向、反向---不翻转电平
    ATIM_OC1AInit(&ATIM_OCInitStructure);   // CH1A比较输出设置
    ATIM_OC1BInit(&ATIM_OCInitStructure);   // CH1B比较输出设置  

    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);  // ATIM中断设置
    ATIM_CH1Config(ATIM_CHxB_CIE | ATIM_CHxA_CIE, ENABLE);  // 设置通道1的功能
    ATIM_SetCompare1A(3200); // PB2 50%占空比
    ATIM_SetCompare1B(2560);    // PB13 40%占空比
    // ATIM_PWMOutputConfig(OCREFA_TYPE_SINGLE, OUTPUT_TYPE_COMP, 1);  // 设置PWM输出的参数
    ATIM_Cmd(ENABLE);   // ATIM 启动   
    ATIM_CtrlPWMOutputs(ENABLE);    // 使能PWM输出
}

