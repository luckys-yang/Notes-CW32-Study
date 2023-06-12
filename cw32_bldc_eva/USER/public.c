/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: ���ò��ִ���
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Memory_Clear(uint8_t*, uint16_t);    // �ڴ����

/*====================================static function declaration area   END====================================*/
Public_t Public = 
{
    Memory_Clear,
};

/*
* @function: Memory_Clear
* @param: puc_Buffer -> Ҫ������ڴ��׵�ַ LEN -> �ڴ泤��(ע���ִ�����strlen() ������sizeof() !!!)
* @retval: None
* @brief: �ڴ����
*/
static void Memory_Clear(uint8_t* puc_Buffer, uint16_t LEN)
{
    uint16_t i = 0;

    for (i = 0; i < LEN; i++)
    {
        *(puc_Buffer + i) = (uint8_t)0;
    }
}
