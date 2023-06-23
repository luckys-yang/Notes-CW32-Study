#ifndef __OLED_096_H
#define __OLED_096_H
#include "main.h"

// OLED的IIC地址(SA0 = 0) --- 0111 1000
#define OLED096_ADDR       0x78
// OLED参数(宽度，高度，页数量)
#define OLED096_WIDTH      128
#define OLED096_HEIGHT     64

typedef enum
{
    ASCII_SIZE_16 = 16,
} ASCII_Size_t;

// 最大只能显示4行
typedef enum
{
    OLED096_Line1 = 0,  
    OLED096_Line2 = 2,
    OLED096_Line3 = 4,
    OLED096_Line4 = 6,
} OLED091_Line_t;

typedef struct
{
    // uint8_t OLED096_Display_Buff[4][16];    // OLED显示ASCII数组
    void (*padString)(char *, int); // 补全空格
    void (*OLED096_Init)(void);     // OLED初始化
    void (*OLED096_Clear)(void);    // 清屏
    void (*OLED096_Show_String)(uint8_t, OLED091_Line_t, const char*, ASCII_Size_t);       // OLED显示字符串
    void (*OLED096_Show_CHN)(uint8_t, OLED091_Line_t, const char*);        // OLED显示单个汉字
}OLED096_t;

extern OLED096_t OLED096;


#endif
