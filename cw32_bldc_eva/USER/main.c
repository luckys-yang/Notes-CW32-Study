/***************************************************************************
 * File: main.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: CW32_BLDC_EVAѧϰ
****************************************************************************/
#include "main.h"

int main(void)
{
	System_Init.Hardware_Init();

	while (1)
	{
		System.Run();
	}
}
