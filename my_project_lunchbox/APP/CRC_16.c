/***************************************************************************
 * File: CRC_16.c
 * Author: Luckys.
 * Date: 2023/06/12
 * description: CRC_16校验
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static uint16_t CRC_Check(uint8_t *, uint8_t); //CRC校验

/*====================================static function declaration area   END====================================*/
CRC_16_t  CRC_16 = 
{
    0, 
    0, 
    0, 
    CRC_Check
};

/*
* @function: CRC_Check
* @param: CRC_Ptr -> 数组指针 LEN -> 数组长度(使用sizeof)
* @retval: None
* @brief: CRC校验值
*/
static uint16_t CRC_Check(uint8_t *CRC_Ptr, uint8_t LEN)
{
    uint16_t CRC_Value = 0;
    uint8_t i = 0;
    uint8_t j = 0;

    CRC_Value = 0xffff;
    for (i = 0; i < LEN; i++)
    {
        CRC_Value ^= *(CRC_Ptr + i);
        for (j = 0; j < 8; j++)
        {
            if (CRC_Value & 0x00001)
                CRC_Value = (CRC_Value >> 1) ^ 0xA001;
            else
                CRC_Value = (CRC_Value >> 1);
        }
    }
    CRC_Value = ((CRC_Value >> 8) + (CRC_Value << 8)); // 交换高低字节

    return CRC_Value;
}

