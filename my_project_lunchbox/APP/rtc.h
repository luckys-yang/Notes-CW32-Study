#ifndef __RTC_H
#define __RTC_H
#include "main.h"

typedef enum
{
    YEAR = (uint8_t)0,
    MON = (uint8_t)1,
    DAY = (uint8_t)2,
    HOUR = (uint8_t)3,
    MIN = (uint8_t)4,
    SEC = (uint8_t)5,    
}myRTC_Buff_Index_t;

typedef struct
{
    uint16_t usRtc_Buff[6]; // 存储年月日时分秒
    void (*myRTC_Init)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);   // RTC初始化
    void (*myRTC_Refresh)(void);    // RTC刷新
    void (*myRTC_Set_Time)(uint8_t, uint8_t, uint8_t);  // 设置时间
    void (*myRTC_Set_Date)(uint16_t, uint8_t, uint8_t,uint8_t);    // 设置日期
    void (*myRTC_Alarm_A_Init)(void);   // 闹钟A初始化
}myRTC_t;


extern myRTC_t myRTC;


#endif
