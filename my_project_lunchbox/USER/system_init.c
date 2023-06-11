/***************************************************************************
 * File: system_init.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ϵͳ��ʼ��
****************************************************************************/

#include "main.h"

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
    RCC_Config();
    InitTick(64000000);
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
