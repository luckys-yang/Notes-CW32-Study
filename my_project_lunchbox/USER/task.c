/***************************************************************************
 * File: task.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 任务调度
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void TasksHandle_10MS(void); // 任务
static void TasksHandle_20MS(void); // 任务
static void TasksHandle_100MS(void); // 任务
static void TasksHandle_250MS(void); // 任务
static void TasksHandle_1S(void); // 任务
static void TasksHandle_1p5S(void); // 任务


/*====================================static function declaration area   END====================================*/

Task_t Task[] = 
{
    {FALSE, 10, 10, TasksHandle_10MS},  // task Period: 10ms
    {FALSE, 100, 100, TasksHandle_100MS},  // task Period: 20ms
    {FALSE, 20, 20, TasksHandle_20MS},  // task Period: 20ms
    {FALSE, 250, 250, TasksHandle_250MS},  // task Period: 250ms  
    {FALSE, 1000, 1000, TasksHandle_1S},  // task Period: 1s  
    {FALSE, 1500, 1500, TasksHandle_1p5S},  // task Period: 1.5s  
};

/*====================================variable definition declaration area BEGIN===================================*/

// 最大任务数量
uint8_t ucTasks_Max = sizeof(Task) / sizeof(Task[0]);

/*====================================variable definition declaration area   END===================================*/

/*
* @function: TasksHandle_10MS
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_10MS(void)
{
    Key_1.Key_1_Scan();
    Key_1.Key_1_Handler();
}

/*
* @function: TasksHandle_20MS
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_20MS(void)
{

#ifdef USE_096_OLED     
    switch(Menu.Now_Page_Status)
    {
        case PAGE_TempHum:Menu.Menu_Page1();break;
        case PAGE_RtcTimeDate:Menu.Menu_Page2();break;
        case PAGE_ADC:Menu.Menu_Page3();break;
        case PAGE_PWM:Menu.Menu_Page4();break;
        default:Menu.Now_Page_Status = PAGE_TempHum;break;
    }
#endif    
}

/*
* @function: TasksHandle_100MS
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_100MS(void)
{
#ifdef USE_ADC_Single_One
    ADC_1.ADC1_Single_Channel_One_Convert();    
#elif defined(USE_ADC_Serial_Scan)
    ADC_1.ADC1_Serial_Scan_Convert();
#endif
    if (Menu.Now_Page_Status == PAGE_ADC)
    {
        // OLED刷新
        // printf("A---%.1f\r\n",ADC_1.ADC_Single_Result);
        sprintf((char*)Page3.OLED096_Display_Buff[0],"B0:%.2f A4:%.2f",ADC_1.ADC_Serial_Result_Arr[0], ADC_1.ADC_Serial_Result_Arr[1]);
        OLED096.padString((char*)Page3.OLED096_Display_Buff[0],16);
        sprintf((char*)Page3.OLED096_Display_Buff[1],"A5:%.2f A6:%.2f",ADC_1.ADC_Serial_Result_Arr[2], ADC_1.ADC_Serial_Result_Arr[3]);
        OLED096.padString((char*)Page3.OLED096_Display_Buff[1],16);
    }
}

/*
* @function: TasksHandle_250MS
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_250MS(void)
{
    Led.Led_Flip(LED1);
    if (Menu.Now_Page_Status == PAGE_PWM)
    {
        Gtim.Gtim1_Calculate(); // 计算
    }  
}
/*
* @function: TasksHandle_1S
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_1S(void)
{
    Led.Led_Flip(LED2);
    if (Menu.Now_Page_Status == PAGE_RtcTimeDate)
    {    
        myRTC.myRTC_Refresh();
    }
}

/*
* @function: TasksHandle_1p5S
* @param: None
* @retval: None
* @brief: 任务
*/
static void TasksHandle_1p5S(void)
{
    if (Menu.Now_Page_Status == PAGE_TempHum)
    {    
        DHT11.DHT11_Read_Data(&DHT11.DHT11_Temperture,&DHT11.DHT11_Humidity); // 获取一次温湿度
    }
}
