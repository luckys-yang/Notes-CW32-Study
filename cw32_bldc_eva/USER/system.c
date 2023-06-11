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
