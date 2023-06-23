#ifndef __GHTIM_H
#define __GHTIM_H
#include "main.h"

typedef struct
{
    uint32_t Gtim2_IC_Fre;  // GTIM2输入捕获频率
    uint32_t Gtim2_IC_Duty; // GTIM2输入捕获占空比
    uint16_t Gtim1_CH_Fre; // GTIM频率
    float Gtim1_CH_Duty[2];  // GTIM1 通道占空比 
    uint16_t Gtim1_CH_Set_Pulse[2];   // 设置占空比(2个通道的)
    void (*Gtim1_PWM_Output_Init)(uint16_t, uint16_t);   // 通用定时器1PWM输出初始化
    void (*Gtim1_Calculate)(void);  // 计算频率占空比
    void (*Gtim2_PWM_IC_Init)(void);    // 输入捕获初始化
    void (*Gtim3_PWM_Toggle_Init)(uint16_t, uint16_t);  // 互补初始化
}Gtim_t;



extern Gtim_t Gtim;

#endif
