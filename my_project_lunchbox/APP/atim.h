#ifndef __ATIM_H
#define __ATIM_H
#include "main.h"

typedef struct
{
    void (*Atim_Base_Init)(uint16_t, uint16_t); // ATIM基本定时初始化
    void (*Atim_PWM_Input_Init)(void);  // 输入捕获初始化
    void (*Atim_PWM_Output_OC_Init)(void);  // 输出互补PWM信号初始化
}Atim_t;

extern Atim_t Atim;

#endif
