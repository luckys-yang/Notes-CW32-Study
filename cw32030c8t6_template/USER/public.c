/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 公用部分代码
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Memory_Clear(uint8_t*, uint16_t);    // 内存清除

/*====================================static function declaration area   END====================================*/
Public_t Public = 
{
    Memory_Clear,
};

/*
* @function: Memory_Clear
* @param: puc_Buffer -> 要清除的内存首地址 LEN -> 内存长度(注意字串符用strlen() 数组用sizeof() !!!)
* @retval: None
* @brief: 内存清除
*/
static void Memory_Clear(uint8_t* puc_Buffer, uint16_t LEN)
{
    uint16_t i = 0;

    for (i = 0; i < LEN; i++)
    {
        *(puc_Buffer + i) = (uint8_t)0;
    }
}
