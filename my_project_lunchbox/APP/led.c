/***************************************************************************
 * File: led.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: LED
****************************************************************************/

#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Led_Init(void);      // LED初始化
static void Led_ON(Led_Num_t);   // 打开
static void Led_OFF(Led_Num_t);  // 关闭
static void Led_Flip(Led_Num_t); // 翻转

/*====================================static function declaration area   END====================================*/

Led_t Led = 
{
    Led_Init,
    Led_ON,
    Led_OFF,
    Led_Flip,
};

/*
* @function: Led_Init
* @param: None
* @retval: None
* @brief: LED初始化
*/
static void Led_Init(void)
{
    
}