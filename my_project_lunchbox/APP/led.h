#ifndef __LED_H
#define __LED_H
#include "main.h"

// ����ö������
typedef enum
{
    LED1 = (uint8_t)0x01,
    LED2 = (uint8_t)0x02,
    LED3 = (uint8_t)0x03,
} Led_Num_t;

typedef struct
{
    void (*Led_Init)(void); // LED��ʼ��
    void (*Led_ON)(Led_Num_t);   // ��
    void (*Led_OFF)(Led_Num_t);  // �ر�
    void (*Led_Flip)(Led_Num_t); // ��ת
} Led_t;

extern Led_t Led;

#endif

