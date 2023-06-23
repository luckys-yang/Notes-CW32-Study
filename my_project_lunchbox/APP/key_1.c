/***************************************************************************
 * File: key_1.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: �װ��������(�͵�ƽ��Ч)
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
static void Key_1_Handler(void);    // ��������
/*====================================static function declaration area   END====================================*/

Key_1_t Key_1 = 
{
    0,
    {FALSE},
    Key_1_Init,
    Key_1_Scan,
    Key_1_Handler,
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

/*
* @function: Key_1_Handler
* @param: None
* @retval: None
* @brief: ��������
*/
static void Key_1_Handler(void)
{
    if (Key_1.vucKey_1_Flag_Arr[0]) // K1�̰�
    {
        if (Buzzer.Buzzer_Status == Buzzer_Status_OFF)
        {
            Buzzer.Buzzer_ON();
        }
        else
        {
            Buzzer.Buzzer_OFF();
        }
        Key_1.vucKey_1_Flag_Arr[0] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[1])    // K2�̰�
    {
        // �л�ҳ��
        Menu.Now_Page_Status = (Menu.Now_Page_Status % PAGE_MAX) + 1;
        OLED096.OLED096_Clear();
        Key_1.vucKey_1_Flag_Arr[1] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[2])    //K3�̰�
    {
        Key_1.vucKey_1_Flag_Arr[2] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[3])    // K1����
    {
        Gtim.Gtim1_CH_Set_Pulse[0] += 20;   // ռ�ձȼ�5%
        Gtim.Gtim1_CH_Set_Pulse[1] += 20;   // ռ�ձȼ�5%

        if (Gtim.Gtim1_CH_Set_Pulse[0] > 380)
        {
            Gtim.Gtim1_CH_Set_Pulse[0] = 20;    // ռ�ձȻָ���5%
        }
        if (Gtim.Gtim1_CH_Set_Pulse[1] > 380)
        {
            Gtim.Gtim1_CH_Set_Pulse[1] = 40;    // ռ�ձȻָ���10%
        }
        // ����ռ�ձ�
        CW_GTIM1->CCR1 = Gtim.Gtim1_CH_Set_Pulse[0];
        CW_GTIM1->CCR2 = Gtim.Gtim1_CH_Set_Pulse[1];
        Key_1.vucKey_1_Flag_Arr[3] = FALSE;
    }
    else if (Key_1.vucKey_1_Flag_Arr[4])    // K2����
    {
        Key_1.vucKey_1_Flag_Arr[4] = FALSE;
    }    
}
