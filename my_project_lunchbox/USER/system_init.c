/***************************************************************************
 * File: system_init.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 系统初始化
****************************************************************************/

#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/

uint8_t Init_Cnt = 10;  // 初始化超时等待计数

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void Hardware_Init(void);    // 硬件初始化
static void RCC_Config(void);   // 时钟配置
/*====================================static function declaration area   END====================================*/



System_Init_t System_Init = 
{
    Hardware_Init,
};

/*
* @function: Hardware_Init
* @param: None
* @retval: None
* @brief: 硬件初始化
*/
static void Hardware_Init(void)
{
    Public.System_MS_Delay(1000);   // DHT11上电后至少需要延时1s等待稳定
    RCC_Config();   // 时钟配置
    InitTick(64000000); // SYSTICK初始化
    Btim.BTIM1_Init(1999, BTIM_PRS_DIV32);  // 基本定时器1初始化(计算：64000000 / 2000 / 32 = 1000Hz --- 1 / 1000Hz = 0.001s)    
    Led.Led_Init(); // LED初始化
    Key_1.Key_1_Init(); // 按键初始化
    Key_2.Key_2_Init(); // 按键初始化
    Buzzer.Buzzer_Init();   // 蜂鸣器初始化
#ifdef USE_TFT 
    ST7735.ST7735_Init();   // ST7735 LCD屏幕初始化
#elif defined(USE_096_OLED)
    I2C_Soft.I2C_Init();   // I2C初始化
    OLED096.OLED096_Init(); // 0.96寸OLED初始化
#elif defined(USE_091_OLED)
    // 0.91寸OLED代码待添加
#endif
#ifdef USE_ADC_Single_One 
    ADC_1.ADC1_Single_Channel_One_Init();   // ADC单通道单次采集初始化    
#elif defined(USE_ADC_Serial_Scan) 
    ADC_1.ADC1_Serial_Scan_Init();  // ADC序列扫描初始化
#endif     
    USART1.USART1_Init();   // 串口1初始化
    myRTC.myRTC_Init(2023, RTC_Month_June, 16, RTC_Weekday_Friday, 18, 50, 20); // RTC初始化  
    myRTC.myRTC_Alarm_A_Init(); // 闹钟初始化
    while (!DHT11.DHT11_Init() && (Init_Cnt--)) // DHT11初始化
    {
        Public.System_MS_Delay(100);
    }
    DHT11.DHT11_Read_Data(&DHT11.DHT11_Temperture,&DHT11.DHT11_Humidity); // 获取一次温湿度
    Gtim.Gtim1_PWM_Output_Init(399, GTIM_PRESCALER_DIV64);    // PWM输出初始化 64000000 / 64 / 400 = 2500Hz (即2.5ms) 
#ifdef USE_PWM_IC 
    Gtim.Gtim2_PWM_IC_Init();   // 输入捕获初始化
#endif
#ifdef USE_GTIM3_TOGG
    Gtim.Gtim3_PWM_Toggle_Init(399, GTIM_PRESCALER_DIV64);   // 互补初始化 64000000 / 1024 / 12500 = 5hz (即200ms)  
#endif    
    // Atim.Atim_Base_Init(1999, ATIM_Prescaler_DIV32);    // 高级定时器1初始化(计算：64000000 / 2000 / 32 = 1000Hz --- 1 / 1000Hz = 0.001s) 
#ifdef USE_PWM_IC_ATIM   
    Atim.Atim_PWM_Input_Init();
#endif
#ifdef USE_ATIM_PWM       
    Atim.Atim_PWM_Output_OC_Init();
#endif    
    printf("初始化完成\r\n");
}

/*
* @function: RCC_Config
* @param: None
* @retval: None
* @brief: 时钟配置
*/
static void RCC_Config(void)
{
    /* 0. HSI使能并校准 */
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

    /* 2. 使能PLL，通过PLL倍频到64MHz */
    RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8); // HSI 默认输出频率8MHz
    // RCC_PLL_OUT();  //PC13脚输出PLL时钟

    ///< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
    ///< 当使用的时钟源HCLK大于48MHz：设置FLASH 读等待周期为3 cycle
    __RCC_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_3);

    /* 3. 时钟切换到PLL */
    RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
    RCC_SystemCoreClockUpdate(64000000);
}
