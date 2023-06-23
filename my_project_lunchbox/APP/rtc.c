/***************************************************************************
 * File: rtc.c
 * Author: Luckys.
 * Date: 2023/06/15
 * description: RTC
****************************************************************************/
#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/

// RTC时间日期存储结构体
RTC_TimeTypeDef nTime;
RTC_DateTypeDef nDate;
// 星期
static uint8_t *WeekdayStr[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void myRTC_Init(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
static void myRTC_Calculate_Date(uint16_t, uint8_t, uint8_t*);    // RTC日期计算(十进制 to BCD码)
static void myRTC_Calculate_Time(uint8_t, uint8_t, uint8_t, uint8_t*); // RTC时间计算(十进制 to BCD码)
static void myRTC_Refresh(void);    // RTC刷新
static void myRTC_Set_Time(uint8_t, uint8_t, uint8_t);  // 设置RTC时间
static void myRTC_Set_Date(uint16_t, uint8_t, uint8_t,uint8_t);  // 设置RTC日期
static void myRTC_Alarm_A_Init(void);   // 闹钟A初始化

/*====================================static function declaration area   END====================================*/
myRTC_t myRTC = 
{
    {0},
    myRTC_Init,
    myRTC_Refresh,
    myRTC_Set_Time,
    myRTC_Set_Date,
    myRTC_Alarm_A_Init,
};

/*
* @function: myRTC_Init
* @param: 年 月(RTC_Month_June) 日 星期(RTC_Weekday_Friday) 时 分 秒
* @retval: None
* @brief: RTC初始化
*/
static void myRTC_Init(uint16_t year, uint8_t mon, uint8_t day, uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_InitTypeDef RTC_InitStructure;

    uint8_t Date_temp[2] = {0};  // 按顺序存储： 【年，日】
    uint8_t Time_temp[3] = {0};  // 按顺序存储： 【时，分，秒】

    __RCC_RTC_CLK_ENABLE();
    RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);    // 选择LSE为RTC时钟
    
    myRTC_Calculate_Date(year,day,Date_temp);    // 计算日期
    myRTC_Calculate_Time(hour,min,sec,Time_temp);   // 计算时间

    // 设置日期，DAY、MONTH、YEAR必须为BCD方式，星期为0~6，代表星期日，星期一至星期六
    RTC_InitStructure.DateStruct.Year = Date_temp[0];   // 【年】
    RTC_InitStructure.DateStruct.Month = mon;    // 【月】
    RTC_InitStructure.DateStruct.Day = Date_temp[1];    // 【日】
    RTC_InitStructure.DateStruct.Week = week; // 【星期】
    // 打印测试
    printf("-----Set Date as 20%x/%x/%x\r\n",RTC_InitStructure.DateStruct.Year, RTC_InitStructure.DateStruct.Month, RTC_InitStructure.DateStruct.Day);
    // 设置时间，HOUR、MINIUTE、SECOND必须为BCD方式，须保证HOUR、AMPM、H24之间的关联正确性
    RTC_InitStructure.TimeStruct.Hour = Time_temp[0];   // 【时】
    RTC_InitStructure.TimeStruct.Minute = Time_temp[1]; // 【分】
    RTC_InitStructure.TimeStruct.Second = Time_temp[2]; // 【秒】
    RTC_InitStructure.TimeStruct.AMPM = 0;
    RTC_InitStructure.TimeStruct.H24 = 1; // 24小时制(1) 12小时制(0)   -- 24小时
    RTC_InitStructure.RTC_ClockSource = RTC_RTCCLK_FROM_LSE;    // 时钟源选择 -- 外部晶体时钟LSE
    // 打印测试
    printf("-----Set Time as %02x:%02x:%02x\r\n",RTC_InitStructure.TimeStruct.Hour,RTC_InitStructure.TimeStruct.Minute,RTC_InitStructure.TimeStruct.Second);
    
    RTC_Init(&RTC_InitStructure);
#ifdef USE_RTC_Interrupt
    RTC_SetInterval(RTC_INTERVAL_EVERY_1S); // RTC秒中断
    RTC_ITConfig(RTC_IT_INTERVAL,ENABLE); // 开启RTC中断
    NVIC_EnableIRQ(RTC_IRQn);    // 开启RTC中断
#endif    
}

/*
* @function: myRTC_Alarm_A_Init
* @param: None
* @retval: None
* @brief: 闹钟A初始化
*/
static void myRTC_Alarm_A_Init(void)
{
    RTC_AlarmTypeDef RTC_AlarmStructure;

    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All & (~(RTC_AlarmMask_Seconds|RTC_AlarmMask_Minutes|RTC_AlarmMask_Hours)); // 时分秒吻合触发
    RTC_AlarmStructure.RTC_AlarmTime.AMPM = 0;
    RTC_AlarmStructure.RTC_AlarmTime.H24 = 1;
    RTC_AlarmStructure.RTC_AlarmTime.Hour = 0x06;
    RTC_AlarmStructure.RTC_AlarmTime.Minute = 0x30;
    RTC_AlarmStructure.RTC_AlarmTime.Second = 0x00;

    RTC_SetAlarm(RTC_Alarm_A,&RTC_AlarmStructure);  // 设置闹钟
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);   // 使能闹钟

    RTC_ITConfig(RTC_IT_ALARMA,ENABLE); // 开启闹钟中断
    NVIC_EnableIRQ(RTC_IRQn);   // 开启RTC中断
}

/*
* @function: myRTC_Calculate_Date
* @param: 年 日 存储结果的数组
* @retval: None
* @brief: RTC日期计算(十进制 to BCD码)
*/
static __inline void myRTC_Calculate_Date(uint16_t year, uint8_t day, uint8_t* p_temp)
{
    *(p_temp + 0) = ((year / 1000) << 4) | ((year / 100) % 10) | ((year / 10) % 10) << 4 | (year % 10); // 【年】
    *(p_temp + 1) = ((day / 10) % 10) << 4 | (day % 10);    // 【日】
}

/*
* @function: myRTC_Calculate_Time
* @param: 时 分 秒 存储结果的数组
* @retval: None
* @brief: RTC时间计算(十进制 to BCD码)
*/
static __inline void myRTC_Calculate_Time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t* p_temp)
{
    *(p_temp + 0) = ((hour / 10) % 10) << 4 | (hour % 10);  // 【时】
    *(p_temp + 1) = ((min / 10) % 10) << 4 | (min % 10);    // 【分】
    *(p_temp + 2) = ((sec / 10) % 10) << 4 | (sec % 10);    // 【秒】
}

/*
* @function: myRTC_Refresh
* @param: None
* @retval: None
* @brief: RTC刷新
*/
static void myRTC_Refresh(void)
{
    RTC_GetDate(&nDate); // 获取日期
    RTC_GetTime(&nTime); // 获取时间

    // OLED刷新
    sprintf((char *)Page2.OLED096_Display_Buff[0], "Date:20%02x/%02x/%02x", nDate.Year, nDate.Month, nDate.Day);   // 日期
    OLED096.padString((char *)Page2.OLED096_Display_Buff[0], 16);                                                  // 补全空格
    sprintf((char *)Page2.OLED096_Display_Buff[1], "Time:%02x-%02x-%02x", nTime.Hour, nTime.Minute, nTime.Second); // 时间
    OLED096.padString((char *)Page2.OLED096_Display_Buff[1], 16);                                                  // 补全空格
    sprintf((char *)Page2.OLED096_Display_Buff[2], "Week:%s", WeekdayStr[nDate.Week]);                             // 星期
    OLED096.padString((char *)Page2.OLED096_Display_Buff[2], 16);                                                  // 补全空格
}

/*
* @function: myRTC_Set_Time
* @param: 时分秒
* @retval: None
* @brief: 设置RTC时间
*/
static void myRTC_Set_Time(uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_TimeTypeDef sTime;
    uint8_t Time_temp[3] = {0};

    myRTC_Calculate_Time(hour,min,sec,Time_temp); // 计算
    sTime.Hour = Time_temp[0];
    sTime.Minute = Time_temp[1];
    sTime.Second = Time_temp[2];
    sTime.AMPM = 0;
    sTime.H24 = 1;
    RTC_SetTime(&sTime);    // 设置时间   
}

/*
* @function: myRTC_Set_Date
* @param: None
* @retval: None
* @brief: 设置RTC日期
*/
static void myRTC_Set_Date(uint16_t year, uint8_t mon, uint8_t day,uint8_t week)
{
    RTC_DateTypeDef sDate;
    uint8_t Date_temp[2] = {0};

    myRTC_Calculate_Date(year,day,Date_temp); // 计算
    sDate.Year = Date_temp[0];
    sDate.Month = mon;
    sDate.Day = Date_temp[1];
    sDate.Week = week;
    RTC_SetDate(&sDate);    // 设置日期
}


