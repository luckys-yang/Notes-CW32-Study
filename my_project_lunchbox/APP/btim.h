#ifndef __BTIM_H
#define __BTIM_H
#include "main.h"

typedef struct
{
    void (*BTIM1_Init)(uint16_t, uint16_t);   // BTIM1初始化
}Btim_t;

extern Btim_t Btim;


#endif
