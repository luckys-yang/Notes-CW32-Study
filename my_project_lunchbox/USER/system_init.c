/***************************************************************************
 * File: system_init.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 系统初始化
****************************************************************************/

#include "main.h"

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
    RCC_Config();
    InitTick(64000000);
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
