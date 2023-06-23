/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/11
 * description: 中断函数
****************************************************************************/
#include "main.h"

/*
* @function: UART1_IRQHandler
* @param: None
* @retval: None
* @brief: 串口1中断服务函数
*/
void UART1_IRQHandler(void)
{
    if (USART_GetITStatus(CW_UART1, USART_IT_RC) != RESET)
    {
        USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt] = USART_ReceiveData_8bit(CW_UART1);
        if (USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt] == 0x0A)
        {
            if (USART1.puc_Usart1_Rec_Buffer[USART1.ucUsart1_Rx_Cnt - 1] == 0x0D)   // 判断先后接收到 0x0D 0x0A 则当做一帧
            {
#if CALLBACK_Debug                
                // for (uint16_t i = 0; i < USART1.ucUsart1_Rx_Cnt - 1; i++)   // Cnt - 1去除0x0D
                // {
                //     *(USART1.puc_Usart1_Send_Buffer + i) = *(USART1.puc_Usart1_Rec_Buffer + i);
                // }
                // // 发送数据
                // USART1.USART1_Send_Array(USART1.puc_Usart1_Send_Buffer, USART1.ucUsart1_Rx_Cnt - 1);    // Cnt - 1去除0x0D
                // printf("\r\n");
#endif                
                // ModBus协议解析
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
* @brief: BTIM1中断服务函数
*/
void BTIM1_IRQHandler(void)
{
    static uint16_t count_100ms = 0;
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))			//检查BTIM的状态寄存器的状态位是否置位
    {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);	//清除中断标志位
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
* @brief: GPIOB外部中断服务函数
*/
void GPIOB_IRQHandler(void)
{
    if (CW_GPIOB->ISR_f.PIN2)   // 判断哪个引脚触发
    {
        GPIOB_INTFLAG_CLR(bv2); // 清除标志位
        Led.Led_Flip(LED2);
    }
}

/*
* @function: ADC_IRQHandler
* @param: None
* @retval: None
* @brief: ADC中断服务函数
*/
void ADC_IRQHandler(void)
{
    ADC_1.gFlagIrq = CW_ADC->ISR;   // 获取中断标志寄存器
    CW_ADC->ICR = 0x00; // 中断标志清除寄存器
}

/*
* @function: RTC_IRQHandler
* @param: None
* @retval: None
* @brief: RTC中断服务函数
*/
void RTC_IRQHandler(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))  // 闹钟中断触发
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA);
        Buzzer.Buzzer_ON();
    }
#ifdef USE_RTC_Interrupt    
    if (RTC_GetITState(RTC_IT_INTERVAL))    // RTC秒中断触发
    {
        RTC_ClearITPendingBit(RTC_IT_INTERVAL);
    }
#endif    
}

/*
* @function: GTIM1_IRQHandler
* @param: None
* @retval: None
* @brief: 定时器1中断函数
*/
void GTIM1_IRQHandler(void)
{
    // static uint16_t Timer_Cnt;

    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
    // 需要再打开
    // Timer_Cnt++;
    // if (Timer_Cnt >= 800)   // 2.5x400 = 2s
    // {
    //     Timer_Cnt = 0;
    //     Gtim.Gtim1_CH_Set_Pulse[0] += 40; // 占空比+10%
    //     if (Gtim.Gtim1_CH_Set_Pulse[0] > 360)
    //     {
    //         Gtim.Gtim1_CH_Set_Pulse[0] = 40;
    //     }
    //     // GTIM_SetCompare1(CW_GTIM1, Gtim.Gtim1_CH_Set_Pulse[3]);
    //     CW_GTIM1->CCR1 = Gtim.Gtim1_CH_Set_Pulse[3];  // 设置占空比跟上面函数作用一样
    // }
}

/*
* @function: ATIM_IRQHandler
* @param: None
* @retval: None
* @brief: 高级定时器中断服务函数
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
* @brief: GTIM2中断服务函数
*/
void GTIM2_IRQHandler(void)
{
    static uint8_t Status = 0;   // 标志位，用于表示当前处于PWM信号的哪一阶段
    static uint32_t cnt = 0;    // 计数
    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_OV)) // 判断是否为GTIM1计数器溢出中断
    {
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_OV); // 清除计数器溢出中断标志位
        if (Status == 1)                              // 如果当前处于PWM信号的第二阶段
        {
            cnt++;
        }
    }
    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC1)) // 判断是否为GTIM1捕获比较匹配中断
    {
        if (Status == 0) // 如果当前处于PWM信号的第一阶段
        {
            Gtim.Gtim2_IC_Fre = CW_GTIM2->CCR1; // 获取捕获比较器1的值，即为PWM信号的周期
            Status = 1;                                     // 切换至PWM信号的第二阶段
        }
        else if (Status == 1) // 如果当前处于PWM信号的第二阶段
        {
            Gtim.Gtim2_IC_Fre = CW_GTIM2->CCR1 + cnt * 65536 - Gtim.Gtim2_IC_Fre; // 计算PWM信号的周期
            Status = 0;                                                                       // 切换至PWM信号的第一阶段
            cnt = 0;                                                                          // 计数器清零
        }
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC1); // 清除捕获比较器1匹配中断标志位
    }

    if (GTIM_GetITStatus(CW_GTIM2, GTIM_IT_CC2)) // 判断是否为GTIM1捕获比较匹配中断
    {
        if (Status == 1) // 如果当前处于PWM信号的第二阶段
        {
            Gtim.Gtim2_IC_Duty = CW_GTIM2->CCR2 + cnt * 65536 - Gtim.Gtim2_IC_Fre; // 计算PWM信号的占空比
        }
        GTIM_ClearITPendingBit(CW_GTIM2, GTIM_IT_CC2); // 清除捕获比较器2匹配中断标志位
    }
}
#endif
