#ifndef __KEY_1_H
#define __KEY_1_H
#include "main.h"

// �ܽ� K1--PB13 K2--PB14 K3--PB15
#define Key1_Pin    GPIO_PIN_13
#define Key2_Pin    GPIO_PIN_14
#define Key3_Pin    GPIO_PIN_15

// ��ȡ������ƽ
#define READ_KEY1    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_13)
#define READ_KEY2    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)
#define READ_KEY3    GPIO_ReadPin(CW_GPIOB,GPIO_PIN_15)

typedef enum
{
    KEY_NULL  = (uint8_t)0x00,  // �ް������¼�ֵ
    KEY1_DOWN = (uint8_t)0x01,  // ����1���¼�ֵ  
    KEY2_DOWN = (uint8_t)0x02,  // ����2���¼�ֵ
    KEY3_DOWN = (uint8_t)0x03,  // ����3���¼�ֵ
}Key_1_Status_t;

typedef struct
{
    uint16_t volatile vusKey_1_Timer_Count; // ��������
    uint8_t volatile vucKey_1_Flag_Arr[6];  // ������־λ(�̳���)
    void (*Key_1_Init)(void);   // ������ʼ��
    void (*Key_1_Scan)(void);   // ������������---����ɨ��
}Key_1_t;

extern Key_1_t Key_1;


#endif

