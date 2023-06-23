#ifndef __I2C_H
#define __I2C_H
#include "main.h"

// 管脚 SCL--PB6 SDA--PB7
#define OLED_SCL_Pin    GPIO_PIN_6
#define OLED_SDA_Pin    GPIO_PIN_7

//置位与清零SCL管脚
#define	SET_SCL	GPIO_WritePin(CW_GPIOB,OLED_SCL_Pin,GPIO_Pin_SET) 
#define	CLR_SCL	GPIO_WritePin(CW_GPIOB,OLED_SCL_Pin,GPIO_Pin_RESET)
//置位与清零SDA管脚
#define	SET_SDA	GPIO_WritePin(CW_GPIOB,OLED_SDA_Pin,GPIO_Pin_SET)
#define	CLR_SDA	GPIO_WritePin(CW_GPIOB,OLED_SDA_Pin,GPIO_Pin_RESET)
//读SDA管脚状态
#define READ_SDA	GPIO_ReadPin(CW_GPIOB,OLED_SDA_Pin)

typedef enum
{
	ACK	 = GPIO_Pin_RESET,
	NACK = GPIO_Pin_SET,
}ACK_Value_t;

//定义结构体类型
typedef struct
{
	void (*I2C_Init)(void);  //I2C初始化
	void (*I2C_Start)(void); //I2C起始信号
	void (*I2C_Stop)(void);  //I2C停止信号
	ACK_Value_t (*I2C_Write_Byte)(uint8_t);      //I2C写字节
	uint8_t (*I2C_Read_Byte) (ACK_Value_t);  //I2C读字节
}I2C_Soft_t;


extern I2C_Soft_t  I2C_Soft;

#endif
