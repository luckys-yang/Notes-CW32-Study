/***************************************************************************
 * File: modbus.c
 * Author: Luckys.
 * Date: 2023/06/12
 * description: ModBusЭ��
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Protocol_Analysis(USART1_t*);  //Э�����
static void Modbus_Read_Register(USART1_t*);   //���Ĵ���
static void Modbus_Wrtie_Register(USART1_t*);  //д�Ĵ���

/*====================================static function declaration area   END====================================*/
Modbus_t  Modbus = 
{
	1,
	
	Protocol_Analysis
};

/*
* @function: Protocol_Analysis
* @param: UART -> ����1�ṹ��ָ��
* @retval: None
* @brief: ����
*/
static void Protocol_Analysis(USART1_t *UART)
{
    USART1_t *const COM = UART;
    uint8_t i = 0, Index = 0;

    // ���˸������ݣ����ֽ�Ϊmodbus��ַ����8�ֽ�
    for (i = 0; i < USART1_Rec_LENGTH; i++)
    {
        // ����ֵ��ʼ����Modbus.Addr
        if (Index == 0)
        {
            if (*(COM->puc_Usart1_Rec_Buffer + i) != Modbus.Addr)
                // ��������Ĵ���i++������һ��ѭ��
                continue;
        }

        *(COM->puc_Usart1_Rec_Buffer + Index) = *(COM->puc_Usart1_Rec_Buffer + i);

        // �Ѷ�ȡ8���ֽ�
        if (Index == Modbus_Order_LENGTH)
        {
            break;
        }

        Index++;
    }

    // ����CRC-16
    CRC_16.CRC_Value = CRC_16.CRC_Check(COM->puc_Usart1_Rec_Buffer, 6); // ����CRCֵ
    CRC_16.CRC_H = (uint8_t)(CRC_16.CRC_Value >> 8);
    CRC_16.CRC_L = (uint8_t)CRC_16.CRC_Value;

    // У��CRC-16(Ϊ�˼��ݲ�ͬ�����ϵ�Э�飬���ֽ���ǰ���ߵ��ֽ���ǰ)
    if (((*(COM->puc_Usart1_Rec_Buffer + 6) == CRC_16.CRC_L) && (*(COM->puc_Usart1_Rec_Buffer + 7) == CRC_16.CRC_H)) ||
        ((*(COM->puc_Usart1_Rec_Buffer + 6) == CRC_16.CRC_H) && (*(COM->puc_Usart1_Rec_Buffer + 7) == CRC_16.CRC_L)))
    {
        // У���ַ
        if ((*(COM->puc_Usart1_Rec_Buffer + 0)) == Modbus.Addr)
        {
            // ��������
            if ((*(COM->puc_Usart1_Rec_Buffer + 1)) == FunctionCode_Read_Register)
            {
                Modbus_Read_Register(COM);
            }
            else if ((*(COM->puc_Usart1_Rec_Buffer + 1)) == FunctionCode_Write_Register)
            {
                Modbus_Wrtie_Register(COM);
            }
        }
    }
    // �建��
    Public.Memory_Clear(COM->puc_Usart1_Rec_Buffer, USART1_Rec_LENGTH);
    USART1.ucUsart1_Rx_Cnt = 0;
}

/*
* @function: Modbus_Read_Register
* @param: UART -> ����1�ṹ��ָ��
* @retval: None
* @brief: ���Ĵ���
*/
static void Modbus_Read_Register(USART1_t *UART)
{
    USART1_t *const  COM = UART;

    //У���ַ
    if((*(COM->puc_Usart1_Rec_Buffer + 2) == 0x9C) && (*(COM->puc_Usart1_Rec_Buffer + 3) == 0x41))
    {
        ////��Ӧ����
        //��ַ��
        *(COM->puc_Usart1_Send_Buffer + 0)  = Modbus.Addr;
        //������
        *(COM->puc_Usart1_Send_Buffer + 1)  = FunctionCode_Read_Register;
        //���ݳ���
        *(COM->puc_Usart1_Send_Buffer + 2)  = 2;
        //������״̬
        *(COM->puc_Usart1_Send_Buffer + 3)  = 0;
        *(COM->puc_Usart1_Send_Buffer + 4) = Buzzer.Buzzer_Status;

        //����CRC
        CRC_16.CRC_Value = CRC_16.CRC_Check(COM->puc_Usart1_Send_Buffer, 5); //����CRCֵ����ΪCRCǰ��5���ֽ�
        CRC_16.CRC_H     = (uint8_t)(CRC_16.CRC_Value >> 8);
        CRC_16.CRC_L     = (uint8_t)CRC_16.CRC_Value;
		// ��λ��ǰ��λ�ں�
        *(COM->puc_Usart1_Send_Buffer + 5) = CRC_16.CRC_L;
        *(COM->puc_Usart1_Send_Buffer + 6) = CRC_16.CRC_H;

        //��������
        USART1.USART1_Send_Array(COM->puc_Usart1_Send_Buffer, 7);
        printf("\r\n"); // VOFA+��λ����Ҫ�ģ�
    }
}

/*
* @function: Modbus_Wrtie_Register
* @param: UART -> ����1�ṹ��ָ��
* @retval: None
* @brief: ����
*/
static void Modbus_Wrtie_Register(USART1_t *UART)
{
    USART1_t *const COM = UART;
    uint8_t i;

    ////��Ӧ����
    // ׼������
    for (i = 0; i < 8; i++)
    {
        *(COM->puc_Usart1_Send_Buffer + i) = *(COM->puc_Usart1_Rec_Buffer + i);
    }
    // ��������
    USART1.USART1_Send_Array(COM->puc_Usart1_Send_Buffer, 8);
    printf("\r\n"); // VOFA+��λ����Ҫ�ģ�

    // У���ַ -> ������
    if ((*(COM->puc_Usart1_Rec_Buffer + 2) == 0x9C) && (*(COM->puc_Usart1_Rec_Buffer + 3) == 0x44)) // 40004
    {
        // ���Ʒ�����
        if (*(COM->puc_Usart1_Rec_Buffer + 5) == 0x01)
        {
            Buzzer.Buzzer_ON();
        }
        else
        {
            Buzzer.Buzzer_OFF();
        }
    }
}
