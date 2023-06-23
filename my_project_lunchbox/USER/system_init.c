/***************************************************************************
 * File: system_init.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ϵͳ��ʼ��
****************************************************************************/

#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/

uint8_t Init_Cnt = 10;  // ��ʼ����ʱ�ȴ�����

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void Hardware_Init(void);    // Ӳ����ʼ��
static void RCC_Config(void);   // ʱ������
/*====================================static function declaration area   END====================================*/



System_Init_t System_Init = 
{
    Hardware_Init,
};

/*
* @function: Hardware_Init
* @param: None
* @retval: None
* @brief: Ӳ����ʼ��
*/
static void Hardware_Init(void)
{
    Public.System_MS_Delay(1000);   // DHT11�ϵ��������Ҫ��ʱ1s�ȴ��ȶ�
    RCC_Config();   // ʱ������
    InitTick(64000000); // SYSTICK��ʼ��
    Btim.BTIM1_Init(1999, BTIM_PRS_DIV32);  // ������ʱ��1��ʼ��(���㣺64000000 / 2000 / 32 = 1000Hz --- 1 / 1000Hz = 0.001s)    
    Led.Led_Init(); // LED��ʼ��
    Key_1.Key_1_Init(); // ������ʼ��
    Key_2.Key_2_Init(); // ������ʼ��
    Buzzer.Buzzer_Init();   // ��������ʼ��
#ifdef USE_TFT 
    ST7735.ST7735_Init();   // ST7735 LCD��Ļ��ʼ��
#elif defined(USE_096_OLED)
    I2C_Soft.I2C_Init();   // I2C��ʼ��
    OLED096.OLED096_Init(); // 0.96��OLED��ʼ��
#elif defined(USE_091_OLED)
    // 0.91��OLED��������
#endif
#ifdef USE_ADC_Single_One 
    ADC_1.ADC1_Single_Channel_One_Init();   // ADC��ͨ�����βɼ���ʼ��    
#elif defined(USE_ADC_Serial_Scan) 
    ADC_1.ADC1_Serial_Scan_Init();  // ADC����ɨ���ʼ��
#endif     
    USART1.USART1_Init();   // ����1��ʼ��
    myRTC.myRTC_Init(2023, RTC_Month_June, 16, RTC_Weekday_Friday, 18, 50, 20); // RTC��ʼ��  
    myRTC.myRTC_Alarm_A_Init(); // ���ӳ�ʼ��
    while (!DHT11.DHT11_Init() && (Init_Cnt--)) // DHT11��ʼ��
    {
        Public.System_MS_Delay(100);
    }
    DHT11.DHT11_Read_Data(&DHT11.DHT11_Temperture,&DHT11.DHT11_Humidity); // ��ȡһ����ʪ��
    Gtim.Gtim1_PWM_Output_Init(399, GTIM_PRESCALER_DIV64);    // PWM�����ʼ�� 64000000 / 64 / 400 = 2500Hz (��2.5ms) 
#ifdef USE_PWM_IC 
    Gtim.Gtim2_PWM_IC_Init();   // ���벶���ʼ��
#endif
#ifdef USE_GTIM3_TOGG
    Gtim.Gtim3_PWM_Toggle_Init(399, GTIM_PRESCALER_DIV64);   // ������ʼ�� 64000000 / 1024 / 12500 = 5hz (��200ms)  
#endif    
    // Atim.Atim_Base_Init(1999, ATIM_Prescaler_DIV32);    // �߼���ʱ��1��ʼ��(���㣺64000000 / 2000 / 32 = 1000Hz --- 1 / 1000Hz = 0.001s) 
#ifdef USE_PWM_IC_ATIM   
    Atim.Atim_PWM_Input_Init();
#endif
#ifdef USE_ATIM_PWM       
    Atim.Atim_PWM_Output_OC_Init();
#endif    
    printf("��ʼ�����\r\n");
}

/*
* @function: RCC_Config
* @param: None
* @retval: None
* @brief: ʱ������
*/
static void RCC_Config(void)
{
    /* 0. HSIʹ�ܲ�У׼ */
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);

    /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

    /* 2. ʹ��PLL��ͨ��PLL��Ƶ��64MHz */
    RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8); // HSI Ĭ�����Ƶ��8MHz
    // RCC_PLL_OUT();  //PC13�����PLLʱ��

    ///< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
    ///< ��ʹ�õ�ʱ��ԴHCLK����48MHz������FLASH ���ȴ�����Ϊ3 cycle
    __RCC_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_3);

    /* 3. ʱ���л���PLL */
    RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
    RCC_SystemCoreClockUpdate(64000000);
}
