#ifndef __ADC1_H
#define __ADC1_H
#include "main.h"


typedef struct
{
    volatile uint8_t gFlagIrq;  // ADC完成采集中断标志位
    volatile float ADC_Single_Result;    // ADC单次采集单通道转换结果
    volatile float ADC_Serial_Result_Arr[4];  // ADC序列扫描转换结果数组
    void (*ADC1_Single_Channel_One_Init)(void);    // ADC单通道单次采集初始化
    void (*ADC1_Serial_Scan_Init)(void);    // ADC序列扫描采集初始化
    void (*ADC1_Single_Channel_One_Convert)(void); // ADC单通道单次采集转换
    void (*ADC1_Serial_Scan_Convert)(void); // ADC序列扫描采集转换
}ADC_1_t;

extern ADC_1_t ADC_1;

#endif
