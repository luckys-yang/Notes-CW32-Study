#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H
#include "main.h"

typedef struct
{
    void (*Hardware_Init)(void);    // Ӳ����ʼ��
}System_Init_t;


extern System_Init_t System_Init;

#endif

