#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "main.h"

typedef struct
{
    void (*System_Run)(void);  // 系统运行
    void (*Error_Handler)(void);    // 系统错误处理
    void (*Task_Marks_Handler)(void);   // 任务标记
}System_t;


extern System_t System;
#endif
