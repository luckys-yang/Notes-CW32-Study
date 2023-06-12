/***************************************************************************
 * File: system.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ϵͳ����
****************************************************************************/

#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Run(void);  // ϵͳ����
static void Error_Handler(void);    // ϵͳ������

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
* @brief: ϵͳ����
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
* @brief: ϵͳ������
*/
static void Error_Handler(void)
{
    
}
