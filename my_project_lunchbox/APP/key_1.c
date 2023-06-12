/***************************************************************************
 * File: key_1.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ��������
****************************************************************************/

#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/
// ������ֵ��̧��һ˲�䡢����һ˲��
static uint8_t ucKey_1_Value,ucKey_1_Up,ucKey_1_Down;

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void Key_1_Init(void);    // ������ʼ��
static uint8_t Key_1_Return_Value(void); // ���ؼ�ֵ
static void Key_1_Scan(void);   // ������������---����ɨ��
/*====================================static function declaration area   END====================================*/

Key_1_t Key_1 = 
{
    0,
    {FALSE},
    Key_1_Init,
    Key_1_Scan,
};

/*
* @function: Key_1_Init
* @param: None
* @retval: None
* @brief: ������ʼ��
*/
static void Key_1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ʹ��ʱ��
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;          // �ܽ��ж�ģʽ--��
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP; // ģʽ--��������
    GPIO_InitStructure.Pins = Key1_Pin | Key2_Pin | Key3_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}

/*
* @function: Key_1_Return_Value
* @param: None
* @retval: None
* @brief: ���ؼ�ֵ
*/
static uint8_t Key_1_Return_Value(void)
{
    if ((!READ_KEY1) || (!READ_KEY2) || (!READ_KEY3))
    {
        if (!READ_KEY1)
        {
            return KEY1_DOWN;
        }
        else if (!READ_KEY2)
        {
            return KEY2_DOWN;
        }
        else if (!READ_KEY3)
        {
            return KEY3_DOWN;
        }                
    }

    return KEY_NULL;
}

/*
* @function: Key_1_Scan
* @param: None
* @retval: None
* @brief: ������������---����ɨ��
*/
static void Key_1_Scan(void)
{
    static uint8_t uckey_1_old;

    ucKey_1_Value = Key_1_Return_Value();                      // ��ȡ�����ļ�ֵ
    ucKey_1_Up = ~ucKey_1_Value & (uckey_1_old ^ ucKey_1_Value); // �����������ؼ�� ֻ�ڰ���̧���˲����Ч ����ʱ�̶�Ϊ����Ч
    ucKey_1_Down = ucKey_1_Value & (uckey_1_old ^ ucKey_1_Value); // �������½��ؼ�� ֻ�ڰ������µ�˲����Ч ����ʱ�̶�Ϊ����Ч
    uckey_1_old = ucKey_1_Value;                          // ��¼��һ�ΰ������º�ļ�ֵ

    if (ucKey_1_Down) // ���а�������ʱ
    {
        Key_1.vusKey_1_Timer_Count = 0; // ����ʱ������ ���㿪ʼ��ʱ �˴�ʹ���˻�����ʱ�����ڼ�ʱ
    }

    if (Key_1.vusKey_1_Timer_Count < 10) // �����ʱʱ��С��1s �̰�
    {
        switch (ucKey_1_Up) // �жϰ����Ƿ�̧�� ѡ���ִֵ�ж̰�����Ӧ����
        {
            case KEY1_DOWN:Key_1.vucKey_1_Flag_Arr[0] = TRUE;break;
            case KEY2_DOWN:Key_1.vucKey_1_Flag_Arr[1] = TRUE;break;
            case KEY3_DOWN:Key_1.vucKey_1_Flag_Arr[2] = TRUE;break;
            default:break;
        }
    }
    else // ���� ��ʱʱ�䳬��1s
    {
        switch (ucKey_1_Value) // �жϰ����Ƿ�̧�� ѡ���ִֵ�ж̰�����Ӧ����
        {
            case KEY1_DOWN:Key_1.vucKey_1_Flag_Arr[3] = TRUE;break;
            case KEY2_DOWN:Key_1.vucKey_1_Flag_Arr[4] = TRUE;break;
            case KEY3_DOWN:Key_1.vucKey_1_Flag_Arr[5] = TRUE;break;
            default:break;
        }
    }
}

