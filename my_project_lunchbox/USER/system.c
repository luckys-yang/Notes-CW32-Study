/***************************************************************************
 * File: system.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 系统函数
****************************************************************************/

#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Run(void);  // 系统运行
static void Error_Handler(void);    // 系统错误处理

/*====================================static function declaration area   END====================================*/

System_t System = 
{
    Run,
    Error_Handler,
};

/*
* @function: Run
* @param: None
* @retval: None
* @brief: 系统运行
*/
static void Run(void)
{
    Key_1.Key_1_Scan();
    if (Key_1.vucKey_1_Flag_Arr[0])
    {
        if (Buzzer.Buzzer_Status == Buzzer_Status_OFF)
        {
            Buzzer.Buzzer_ON();
        }
        else
        {
            Buzzer.Buzzer_OFF();
        }
        Key_1.vucKey_1_Flag_Arr[0] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[1])
    {
        Led.Led_Flip(LED2);
        Key_1.vucKey_1_Flag_Arr[1] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[2])
    {
        Led.Led_Flip(LED3);
        Key_1.vucKey_1_Flag_Arr[2] = FALSE;
    }        
}

/*
* @function: Error_Handler
* @param: None
* @retval: None
* @brief: 系统错误处理
*/
static void Error_Handler(void)
{
    
}
