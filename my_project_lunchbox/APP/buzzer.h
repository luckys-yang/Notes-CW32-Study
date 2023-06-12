#ifndef __BUZZER_H
#define __BUZZER_H
#include "main.h"

// �ܼҶ��� buzzer--PB3
#define Buzzer_Pin GPIO_PIN_3

typedef enum
{
    Buzzer_Status_ON = (uint8_t)0x01,   // ����������̬
    Buzzer_Status_OFF = (uint8_t)0x00,  // �������ر�̬
}Buzzer_Status_t;

typedef struct
{
    Buzzer_Status_t Buzzer_Status;    // ������״̬
    void (*Buzzer_Init)(void);  // ��������ʼ��
    void (*Buzzer_ON)(void);  // ��������
    void (*Buzzer_OFF)(void);  // �������ر�
}Buzzer_t;

extern Buzzer_t Buzzer;

#endif
