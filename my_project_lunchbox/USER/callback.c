/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: �жϺ���
****************************************************************************/
#include "main.h"

/*
* @function: UART1_IRQHandler
* @param: None
* @retval: None
* @brief: ����1�жϷ�����
*/
void UART1_IRQHandler(void)
{
    if (USART_GetITStatus(CW_UART1, USART_IT_RC) != RESET)
    {
        USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt] = USART_ReceiveData_8bit(CW_UART1);
        if (USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt] == 0x0A)
        {
            if (USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt - 1] == 0x0D)   // �ж��Ⱥ���յ� 0x0D 0x0A ����һ֡
            {
#if CALLBACK_Debug                
                // for (uint16_t i = 0; i < USART1.ucUsart1_Rx_Cnt - 1; i++)   // Cnt - 1ȥ��0x0D
                // {
                //     *(USART1.puc_Usart1_Send_Buffer + i) = *(USART1.puc_Usart1_Rec_Buffer + i);
                // }
                // // ��������
                // USART1.USART1_Send_Array(USART1.puc_Usart1_Send_Buffer, USART1.ucUsart1_Rx_Cnt - 1);    // Cnt - 1ȥ��0x0D
                // printf("\r\n");
#endif                
                // ModBusЭ�����
                Modbus.Protocol_Analysis(&USART1);
            }
            else
            {
                USART1.ucUsart1_Rx_Cnt++;
            }
        }
        else
        {
            USART1.ucUsart1_Rx_Cnt++;
        }
        USART_ClearITPendingBit(CW_UART1, USART_IT_RC);
    }
}

/*
* @function: BTIM1_IRQHandler
* @param: None
* @retval: None
* @brief: BTIM1�жϷ�����
*/
void BTIM1_IRQHandler(void)
{
    static uint16_t count_100ms = 0;
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))			//���BTIM��״̬�Ĵ�����״̬λ�Ƿ���λ
    {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);	//����жϱ�־λ
        count_100ms++;

        System.Task_Marks_Handler();
        if(100 == count_100ms)
        {
            count_100ms = 0;
            Key_1.vusKey_1_Timer_Count++;
        }
    }
}

/*
* @function: GPIOA_IRQHandler
* @param: None
* @retval: None
* @brief: GPIOB�ⲿ�жϷ�����
*/
void GPIOB_IRQHandler(void)
{
    if (CW_GPIOB->ISR_f.PIN2)   // �ж��ĸ����Ŵ���
    {
        GPIOB_INTFLAG_CLR(bv2); // �����־λ
        Led.Led_Flip(LED2);
    }
}

/*
* @function: ADC_IRQHandler
* @param: None
* @retval: None
* @brief: ADC�жϷ�����
*/
void ADC_IRQHandler(void)
{
    ADC_1.gFlagIrq = CW_ADC->ISR;   // ��ȡ�жϱ�־�Ĵ���
    CW_ADC->ICR = 0x00; // �жϱ�־����Ĵ���
}

/*
* @function: RTC_IRQHandler
* @param: None
* @retval: None
* @brief: RTC�жϷ�����
*/
void RTC_IRQHandler(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))  // �����жϴ���
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA);
        Buzzer.Buzzer_ON();
    }
#ifdef USE_RTC_Interrupt    
    if (RTC_GetITState(RTC_IT_INTERVAL))    // RTC���жϴ���
    {
        RTC_ClearITPendingBit(RTC_IT_INTERVAL);
    }
#endif    
}

/*
* @function: GTIM1_IRQHandler
* @param: None
* @retval: None
* @brief: ��ʱ��1�жϺ���
*/
void GTIM1_IRQHandler(void)
{
    // static uint16_t Timer_Cnt;

    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
    // ��Ҫ�ٴ�
    // Timer_Cnt++;
    // if (Timer_Cnt >= 800)   // 2.5x400 = 2s
    // {
    //     Timer_Cnt = 0;
    //     Gtim.Gtim1_CH_Set_Pulse[0] += 40; // ռ�ձ�+10%
    //     if (Gtim.Gtim1_CH_Set_Pulse[0] > 360)
    //     {
    //         Gtim.Gtim1_CH_Set_Pulse[0] = 40;
    //     }
    //     // GTIM_SetCompare1(CW_GTIM1, Gtim.Gtim1_CH_Set_Pulse[3]);
    //     CW_GTIM1->CCR1 = Gtim.Gtim1_CH_Set_Pulse[3];  // ����ռ�ձȸ����溯������һ��
    // }
}

/*
* @function: ATIM_IRQHandler
* @param: None
* @retval: None
* @brief: �߼���ʱ���жϷ�����
*/
void ATIM_IRQHandler(void)
{
    if (ATIM_GetITStatus(ATIM_IT_OVF))
    {
        ATIM_ClearITPendingBit(ATIM_IT_OVF);
    }

    if (ATIM_GetITStatus(ATIM_IT_C1BF))
    {
        ATIM_ClearITPendingBit(ATIM_IT_C1BF);
    }

    if (ATIM_GetITStatus(ATIM_IT_C1AF))
    {
        ATIM_ClearITPendingBit(ATIM_IT_C1AF);
    }
}

#ifdef USE_PWM_IC
/*
* @function: GTIM2_IRQHandler
* @param: None
* @retval: None
* @brief: GTIM2�жϷ�����
*/
void GTIM2_IRQHandler(void)
{
    static uint8_t Status = 0;   // ��־λ�����ڱ�ʾ��ǰ����PWM�źŵ���һ�׶�
    static uint32_t cnt = 0;    // ����
    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_OV)) // �ж��Ƿ�ΪGTIM1����������ж�
    {
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_OV); // �������������жϱ�־λ
        if (Status == 1)                              // �����ǰ����PWM�źŵĵڶ��׶�
        {
            cnt++;
        }
    }
    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC1)) // �ж��Ƿ�ΪGTIM1����Ƚ�ƥ���ж�
    {
        if (Status == 0) // �����ǰ����PWM�źŵĵ�һ�׶�
        {
            Gtim.Gtim2_IC_Fre = CW_GTIM2->CCR1; // ��ȡ����Ƚ���1��ֵ����ΪPWM�źŵ�����
            Status = 1;                                     // �л���PWM�źŵĵڶ��׶�
        }
        else if (Status == 1) // �����ǰ����PWM�źŵĵڶ��׶�
        {
            Gtim.Gtim2_IC_Fre = CW_GTIM2->CCR1 + cnt * 65536 - Gtim.Gtim2_IC_Fre; // ����PWM�źŵ�����
            Status = 0;                                                                       // �л���PWM�źŵĵ�һ�׶�
            cnt = 0;                                                                          // ����������
        }
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC1); // �������Ƚ���1ƥ���жϱ�־λ
    }

    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC2)) // �ж��Ƿ�ΪGTIM1����Ƚ�ƥ���ж�
    {
        if (Status == 1) // �����ǰ����PWM�źŵĵڶ��׶�
        {
            Gtim.Gtim2_IC_Duty = CW_GTIM2->CCR2 + cnt * 65536 - Gtim.Gtim2_IC_Fre; // ����PWM�źŵ�ռ�ձ�
        }
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC2); // �������Ƚ���2ƥ���жϱ�־λ
    }
}
#endif
