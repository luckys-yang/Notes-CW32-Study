/***************************************************************************
 * File: led.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: LED
****************************************************************************/

#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Led_Init(void);      // LED��ʼ��
static void Led_ON(Led_Num_t);   // ��
static void Led_OFF(Led_Num_t);  // �ر�
static void Led_Flip(Led_Num_t); // ��ת

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
* @brief: LED��ʼ��
*/
static void Led_Init(void)
{
    
}