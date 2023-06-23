/***************************************************************************
 * File: main.c
 * Author: Luckys.
 * Date: 2023/06/11
 * description: CW32030C8T6大学板
****************************************************************************/
#include "main.h"

int main(void)
{
  System_Init.Hardware_Init();
  
  while (1)
  {
    System.System_Run();
  }
}
