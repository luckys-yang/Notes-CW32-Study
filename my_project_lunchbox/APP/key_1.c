/***************************************************************************
 * File: key_1.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 独立按键
****************************************************************************/

#include "main.h"

/*====================================variable definition declaration area BEGIN===================================*/
// 按键键值、抬起一瞬间、按下一瞬间
static uint8_t ucKey_1_Value,ucKey_1_Up,ucKey_1_Down;

/*====================================variable definition declaration area   END===================================*/

/*====================================static function declaration area BEGIN====================================*/

static void Key_1_Init(void);    // 按键初始化
static uint8_t Key_1_Return_Value(void); // 返回键值
static void Key_1_Scan(void);   // 按键三行消抖---按键扫描
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
* @brief: 按键初始化
*/
static void Key_1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能时钟
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;          // 管脚中断模式--无
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP; // 模式--上拉输入
    GPIO_InitStructure.Pins = Key1_Pin | Key2_Pin | Key3_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}

/*
* @function: Key_1_Return_Value
* @param: None
* @retval: None
* @brief: 返回键值
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
* @brief: 按键三行消抖---按键扫描
*/
static void Key_1_Scan(void)
{
    static uint8_t uckey_1_old;

    ucKey_1_Value = Key_1_Return_Value();                      // 读取按键的键值
    ucKey_1_Up = ~ucKey_1_Value & (uckey_1_old ^ ucKey_1_Value); // 按键的上升沿检测 只在按键抬起的瞬间有效 其他时刻都为零无效
    ucKey_1_Down = ucKey_1_Value & (uckey_1_old ^ ucKey_1_Value); // 按键的下降沿检测 只在按键按下的瞬间有效 其他时刻都为零无效
    uckey_1_old = ucKey_1_Value;                          // 记录上一次按键按下后的键值

    if (ucKey_1_Down) // 当有按键按下时
    {
        Key_1.vusKey_1_Timer_Count = 0; // 将计时器清零 从零开始计时 此处使用了基础定时器用于计时
    }

    if (Key_1.vusKey_1_Timer_Count < 10) // 如果计时时间小于1s 短按
    {
        switch (ucKey_1_Up) // 判断按键是否抬起 选择键值执行短按的相应程序
        {
            case KEY1_DOWN:Key_1.vucKey_1_Flag_Arr[0] = TRUE;break;
            case KEY2_DOWN:Key_1.vucKey_1_Flag_Arr[1] = TRUE;break;
            case KEY3_DOWN:Key_1.vucKey_1_Flag_Arr[2] = TRUE;break;
            default:break;
        }
    }
    else // 长按 计时时间超过1s
    {
        switch (ucKey_1_Value) // 判断按键是否抬起 选择键值执行短按的相应程序
        {
            case KEY1_DOWN:Key_1.vucKey_1_Flag_Arr[3] = TRUE;break;
            case KEY2_DOWN:Key_1.vucKey_1_Flag_Arr[4] = TRUE;break;
            case KEY3_DOWN:Key_1.vucKey_1_Flag_Arr[5] = TRUE;break;
            default:break;
        }
    }
}

