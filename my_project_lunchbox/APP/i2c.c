/***************************************************************************
 * File: i2c.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: I2C协议
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void I2C_Delay(void);    // I2C延时
static void I2C_Init(void);  //I2C初始化
static void I2C_Start(void); //I2C起始信号
static void I2C_Stop(void);  //I2C停止信号
static ACK_Value_t I2C_Write_Byte(uint8_t);      //I2C写字节
static uint8_t I2C_Read_Byte(ACK_Value_t);  //I2C读字节

/*====================================static function declaration area   END====================================*/
I2C_Soft_t I2C_Soft = 
{
	I2C_Init,
    I2C_Start,
    I2C_Stop,
	I2C_Write_Byte,
	I2C_Read_Byte
};

/*
* @function: I2C_Delay
* @param: None
* @retval: None
* @brief: 大概延时7us
*/
static void I2C_Delay(void)
{
    uint8_t i;

    for(i = 0; i < 15; i++);
}

/*
* @function: I2C_Init
* @param: None
* @retval: None
* @brief: I2C初始化
*/
static void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能时钟
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;          // 管脚中断模式--无
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD; // 模式--开漏输出
    GPIO_InitStructure.Pins = OLED_SCL_Pin | OLED_SDA_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    SET_SCL;
    SET_SDA;
}

/*
* @function: I2C_Start
* @param: None
* @retval: None
* @brief: I2C开始信号
*/
static void I2C_Start(void)
{
    //SCL为高电平，SDA的下降沿为I2C起始信号
    SET_SDA;
    SET_SCL;
    I2C_Delay();

    CLR_SDA;
    I2C_Delay();

    CLR_SCL;
    I2C_Delay();
}

/*
* @function: I2C_Stop
* @param: None
* @retval: None
* @brief: I2C停止信号
*/
static void I2C_Stop(void)
{
    //SCL为高电平，SDA的上升沿为I2C停止信号
    CLR_SDA;
    SET_SCL;
    I2C_Delay();

    I2C_Delay();
    SET_SDA;
}

/*
* @function: I2C_Write_Byte
* @param: WR_Byte -> 待写入数据
* @retval: ACK_Value_t -> 从机应答值
* @brief: I2C写字节
*/
static ACK_Value_t I2C_Write_Byte(uint8_t WR_Byte)
{
    uint8_t i;
    // 存储读取的SDA电平状态
    ACK_Value_t ACK_Rspond;

    // SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
    // 数据按8位传输，高位在前，利用for循环逐个接收
    for (i = 0; i < 8; i++)
    {
        // SCL清零，主机SDA准备数据
        CLR_SCL;
        I2C_Delay();
        if (WR_Byte & BIT7)
        {
            SET_SDA;
        }
        else
        {
            CLR_SDA;
        }
        I2C_Delay();
        // SCL置高，传输数据
        SET_SCL;
        I2C_Delay();

        // 准备发送下一比特位
        WR_Byte <<= 1;
    }

    CLR_SCL;
    // 释放SDA，等待从机应答
    SET_SDA;
    I2C_Delay();

    SET_SCL;
    I2C_Delay();

    ACK_Rspond = (ACK_Value_t)READ_SDA;

    CLR_SCL;
    I2C_Delay();

    // 返回从机的应答信号
    return ACK_Rspond;
}

/*
* @function: I2C_Read_Byte
* @param: ACK_Value -> 主机回应值
* @retval: 从机返回值
* @brief: I2C读字节
*/
static uint8_t I2C_Read_Byte(ACK_Value_t ACK_Value)
{
    uint8_t RD_Byte = 0, i;

    ////接收数据
    // SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
    // 数据按8位传输，高位在前，利用for循环逐个接收
    for (i = 0; i < 8; i++)
    {
        // 准备接收下一比特位
        RD_Byte <<= 1;

        // SCL清零，从机SDA准备数据
        CLR_SCL;
        I2C_Delay();

        // SCL置高，获取数据
        SET_SCL;
        I2C_Delay();

        RD_Byte |= READ_SDA;
    }

    // SCL清零，主机准备应答信号
    CLR_SCL;
    I2C_Delay();

    // 主机发送应答信号
    if (ACK_Value == ACK)
    {
        CLR_SDA;
    }
    else
    {
        SET_SDA;
    }
    I2C_Delay();
    SET_SCL;
    I2C_Delay();

    // Note:
    // 释放SDA数据线
    // SCL先清零，再释放SDA，防止连续传输数据时，从机错将SDA释放信号当成NACk信号
    CLR_SCL;
    SET_SDA;
    I2C_Delay();

    // 返回数据
    return RD_Byte;
}
