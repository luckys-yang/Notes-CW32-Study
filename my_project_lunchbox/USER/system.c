/***************************************************************************
 * File: system.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ϵͳ����
****************************************************************************/

#include "main.h"


/*====================================static function declaration area BEGIN====================================*/
static void System_Run(void);  // ϵͳ����
static void Error_Handler(void);    // ϵͳ������
static void Task_Marks_Handler(void);   // �����Ǻ���
static void Task_Pro_Handler(void); // ��������

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
* @brief: ϵͳ����
*/
static void System_Run(void)
{
    Task_Pro_Handler();
}

/*
* @function: Error_Handler
* @param: None
* @retval: None
* @brief: ϵͳ������
*/
static void Error_Handler(void)
{
    Buzzer.Buzzer_ON();
}

/*
* @function: Task_Marks_Handler
* @param: None
* @retval: None
* @brief: �����Ǻ���
*/
static void Task_Marks_Handler(void)
{
    uint8_t i;

    for (i = 0; i < ucTasks_Max; i++)
    {
        if (Task[i].Task_Cnt)   // �жϼ����Ƿ�Ϊ0
        {
            Task[i].Task_Cnt--; // �ݼ�
            if (0 == Task[i].Task_Cnt)  // ������0
            {
                Task[i].Task_Cnt = Task[i].Task_Timer;  // ��װ�ؼ���
                Task[i].Run_Status = TRUE;  // ����ִ��״̬��־��1
            }
        }
    }
}

/*
* @function: Task_Pro_Handler
* @param: None
* @retval: None
* @brief: ��������
*/
static void Task_Pro_Handler(void)
{
    uint8_t i;

    for (i = 0; i < ucTasks_Max; i++)
    {
        if (Task[i].Run_Status) // �ж�ִ��״̬��TRUE--ִ�� FALSE--��ִ��
        {
            Task[i].Run_Status = FALSE;
            Task[i].Task_Hook();    // ִ�к���
        }
    }
}
