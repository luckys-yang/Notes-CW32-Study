/***************************************************************************
 * File: system.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 系统函数
****************************************************************************/

#include "main.h"


/*====================================static function declaration area BEGIN====================================*/
static void System_Run(void);  // 系统运行
static void Error_Handler(void);    // 系统错误处理
static void Task_Marks_Handler(void);   // 任务标记函数
static void Task_Pro_Handler(void); // 任务处理函数

/*====================================static function declaration area   END====================================*/

System_t System = 
{
    System_Run,
    Error_Handler,
    Task_Marks_Handler,
};

/*
* @function: System_Run
* @param: None
* @retval: None
* @brief: 系统运行
*/
static void System_Run(void)
{
    Task_Pro_Handler();
}

/*
* @function: Error_Handler
* @param: None
* @retval: None
* @brief: 系统错误处理
*/
static void Error_Handler(void)
{
    Buzzer.Buzzer_ON();
}

/*
* @function: Task_Marks_Handler
* @param: None
* @retval: None
* @brief: 任务标记函数
*/
static void Task_Marks_Handler(void)
{
    uint8_t i;

    for (i = 0; i < ucTasks_Max; i++)
    {
        if (Task[i].Task_Cnt)   // 判断计数是否为0
        {
            Task[i].Task_Cnt--; // 递减
            if (0 == Task[i].Task_Cnt)  // 计数到0
            {
                Task[i].Task_Cnt = Task[i].Task_Timer;  // 重装载计数
                Task[i].Run_Status = TRUE;  // 任务执行状态标志置1
            }
        }
    }
}

/*
* @function: Task_Pro_Handler
* @param: None
* @retval: None
* @brief: 任务处理函数
*/
static void Task_Pro_Handler(void)
{
    uint8_t i;

    for (i = 0; i < ucTasks_Max; i++)
    {
        if (Task[i].Run_Status) // 判断执行状态：TRUE--执行 FALSE--不执行
        {
            Task[i].Run_Status = FALSE;
            Task[i].Task_Hook();    // 执行函数
        }
    }
}
