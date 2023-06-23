#ifndef __MENU_H
#define __MENU_H
#include "main.h"

// 最大页面数量
#define PAGE_MAX 4

typedef struct
{
    uint8_t OLED096_Display_Buff[4][16];    // OLED显示ASCII数组
}Page_Display_t;

typedef enum
{
    PAGE_TempHum = (uint8_t)1,   // 温湿度采集页面
    PAGE_RtcTimeDate = (uint8_t)2,   // RTC时间日期显示页面
    PAGE_ADC = (uint8_t)3,   // ADC采集页面
    PAGE_PWM = (uint8_t)4,  // PWM普通输出页面
}Page_Status_t;

typedef struct
{
    uint8_t Now_Page_Status;   // 当前显示页面 (默认是1)
    void (*Menu_Page1)(void);   // Page 1
    void (*Menu_Page2)(void);   // Page 2
    void (*Menu_Page3)(void);   // Page 3
    void (*Menu_Page4)(void);   // Page 4
}Menu_t;

extern Menu_t Menu;
extern Page_Display_t Page1;
extern Page_Display_t Page2;
extern Page_Display_t Page3;
extern Page_Display_t Page4;

#endif
