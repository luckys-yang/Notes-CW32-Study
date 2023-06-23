#ifndef __TFT_ST7735_H
#define __TFT_ST7735_H
#include "main.h"

// 引脚定义(详见接线表)
#define ST7735_SCL_Pin GPIO_PIN_6
#define ST7735_SDA_Pin GPIO_PIN_7
#define ST7735_RES_Pin GPIO_PIN_15
#define ST7735_DC_Pin GPIO_PIN_4
#define ST7735_CS_Pin GPIO_PIN_5

// 置1操作(bit set/reset register)
#define	LCD_SCL_SET  	CW_GPIOB->BSRR = ST7735_SCL_Pin
#define	LCD_SDA_SET  	CW_GPIOB->BSRR = ST7735_SDA_Pin
#define	LCD_CS_SET  	CW_GPIOB->BSRR = ST7735_CS_Pin
#define	LCD_DC_SET  	CW_GPIOB->BSRR = ST7735_DC_Pin
#define	LCD_RES_SET  	CW_GPIOA->BSRR = ST7735_RES_Pin
// 置0操作(bit reset register)
#define	LCD_SCL_CLR  	CW_GPIOB->BRR = ST7735_SCL_Pin  
#define	LCD_SDA_CLR  	CW_GPIOB->BRR = ST7735_SDA_Pin 
#define	LCD_CS_CLR  	CW_GPIOB->BRR = ST7735_CS_Pin 
#define	LCD_DC_CLR  	CW_GPIOB->BRR = ST7735_DC_Pin
#define	LCD_RES_CLR  	CW_GPIOA->BRR = ST7735_RES_Pin

// X(宽)Y(高)最大
#define LCD_X_MAX 128
#define LCD_Y_MAX 160
// 屏幕方向(0xC0--正常竖屏 0x00--调转竖屏 )
#define LCD_DIR 0xC0

// 颜色
#define LCD_RED  	0xf800  // 红色
#define LCD_GREEN	0x07e0  // 绿色
#define LCD_BLUE 	0x001f  // 蓝色
#define LCD_WHITE	0xffff  // 白色
#define LCD_BLACK	0x0000  // 黑色
#define LCD_YELLOW  0xFFE0  // 黄色
#define LCD_GRAY0   0xEF7D  //灰色
#define LCD_GRAY1   0x8410  //灰色1
#define LCD_GRAY2   0x4208  //灰色2

// 不常用的函数注释起来(不注释会有警告)，用到再提出去即可
#define NONE_USE_CODE 0

typedef struct
{
    void (*ST7735_Init)(void);  // ST7735屏幕总初始化
    void (*ST7735_CLear)(uint16_t); // 清屏
    void (*GUI_DrawFont_GBK16)(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);   // 显示16x16 ASCII或汉字
    void (*GUI_DrawFont_GBK24)(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);   // 显示24x24 ASCII或汉字
    void (*GUI_DrawFont_Num32)(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);   // 显示数码管数字及符号
}ST7735_t;

extern ST7735_t ST7735;


#endif
