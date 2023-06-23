/***************************************************************************
 * File: dht11.c
 * Author: Luckys.
 * Date: 2023/06/16
 * description: dht11温湿度传感器
 -----------------------------------
接线：
    GND ---- GND
    3.3V ---- 3.3V
    PB1 ---- S
 -----------------------------------
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void DHT11_Rest(void);   // DHT11复位
static uint8_t DHT11_Init(void);   // DHT11初始化
static uint8_t DHT11_Check(void);   // DHT11检测
static uint8_t DHT11_Read_Bit(void);  // DHT11读取一个位
static uint8_t DHT11_Read_Byte(void);   // DHT11读取一个字节
static uint8_t DHT11_Read_Data(float *, uint8_t *); // DHT11读取一次数据

/*====================================static function declaration area   END====================================*/
DHT11_t DHT11 = 
{
    0.0,
    0,
    DHT11_Init,
    DHT11_Read_Data,
};

/*
* @function: DHT11_Rest
* @param: None
* @retval: None
* @brief: DHT11复位
*/
static void DHT11_Rest(void)
{
    DHT11_PIN_OUT;  // 输出
    DHT11_PIN_RESET;    // 拉低
    Public.System_MS_Delay(20);    // 拉低至少18ms
    DHT11_PIN_SET;  // 拉高
    Public.System_10US_Delay(3);   // 拉高至少20~40us
}

/*
* @function: DHT11_Check
* @param: None
* @retval: FALSE -- 未检测到DHT11存在 TRUE -- 存在
* @brief: DHT11检测
*/
static uint8_t DHT11_Check(void)
{
    uint8_t retry = TRUE;
    DHT11_PIN_INPUT;    // 输入
    while (DHT11_Read_Pin && (retry < 10))  // DHT11会拉低40~80us
    {
        retry++;
        Public.System_10US_Delay(1);
    }
    if (retry >= 10)
    {
        return FALSE;
    }
    else
    {
        retry = TRUE;
    }
    while ((!DHT11_Read_Pin) && (retry < 10))   // DHT11拉低后会再拉高40~80us
    {
        retry++;
        Public.System_10US_Delay(1);
    }
    if (retry >= 10)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*
* @function: DHT11_Read_Bit
* @param: None
* @retval: 1/0
* @brief: DHT11读取一个位
*/
static uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;

    while (DHT11_Read_Pin && (retry < 10))  // 等待变成低电平
    {
        retry++;
        Public.System_10US_Delay(1);
    }
    retry = 0;
    while ((!DHT11_Read_Pin) && (retry < 10))  // 等待变成高电平
    {
        retry++;
        Public.System_10US_Delay(1);
    }
    Public.System_10US_Delay(4);    // 进入高电平后延时40us(取一个中间值)
    if (DHT11_Read_Pin) // 判断高低电平，即数据1或0
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
* @function: DHT11_Read_Byte
* @param: None
* @retval: 读到的数据
* @brief: DHT11读取一个字节
*/
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

/*
* @function: DHT11_Read_Data
* @param: None
* @retval: TRUE -- 成功 FALSE -- 失败
* @brief: DHT11读取一次数据
*/
static uint8_t DHT11_Read_Data(float *temp,uint8_t *humi)
{
    char buf[5];
    uint8_t i;

    DHT11_Rest();   // 复位

    if (DHT11_Check())  // 检测成功
    {
        for (i = 0; i < 5; i++) // 读取5个数据
        {
            buf[i] = DHT11_Read_Byte();
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])  // 湿度整数-湿度小数-温度整数-温度小数
        {
            *humi = buf[0];
            *temp = buf[2] + (float)buf[3] / 10.0f; // 合并为一个完整的小数
            // printf("A:%d B:%d C:%d D:%d\r\n",buf[0],buf[1],buf[2],buf[3]);
            // OLED刷新
            sprintf((char*)Page1.OLED096_Display_Buff[0],"Temp:%.1f",*temp);
            OLED096.padString((char*)Page1.OLED096_Display_Buff[0],8);
            sprintf((char*)Page1.OLED096_Display_Buff[1],"Humidity:%d %%",*humi);
            OLED096.padString((char*)Page1.OLED096_Display_Buff[1],16);            
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

/*
* @function: DHT11_Init
* @param: None
* @retval: DHT11的回应 TRUE -- 成功 FALSE -- 失败
* @brief: DHT11初始化
*/
static uint8_t DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    GPIO_InitStructure.Pins = DHT11_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    DHT11_Rest();   // 复位

    return DHT11_Check();   // 等待DHT11回应
}

