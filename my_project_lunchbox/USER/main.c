/***************************************************************************
 * File: main.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 主函数
****************************************************************************/
#include "main.h"

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	__RCC_GPIOB_CLK_ENABLE();
	__RCC_GPIOC_CLK_ENABLE();
	__RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pins = GPIO_PIN_13;	// LED4
  GPIO_Init(CW_GPIOC, &GPIO_InitStruct); 
	
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_7|GPIO_PIN_8,GPIO_Pin_RESET);   //LED灯拉高
	GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_RESET);
}

int main(void)
{
  System_Init.Hardware_Init();
  GPIO_Configuration();

  while (1)
  {
    System.Run();
  }
}
