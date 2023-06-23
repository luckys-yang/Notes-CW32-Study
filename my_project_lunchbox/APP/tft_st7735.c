/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/13
 * description: ST7735 LCD屏
 -----------------------------------
接线：
    单片机GND   ----  屏幕GND
    单片机3.3V  ----  屏幕VCC
    单片机PB6   ----  屏幕SCL(MISO)
    单片机PB7   ----  屏幕SDA(MOSI)
    单片机PA15  ----  屏幕RES
    单片机PB4   ----  屏幕DC
    单片机PB5   ----  屏幕CS(片选)
    单片机3.3V  ----  屏幕BLK     
注意：
    使用TFT屏幕需要在 public.h 打开【USE_TFT】 宏        
 -----------------------------------
****************************************************************************/
#include "main.h"
#include "lcd_font.h"

/*====================================static function declaration area BEGIN====================================*/

static void ST7735_Init(void);  // ST7735屏幕总初始化
static void ST7735_GPIO_Init(void); // ST7735屏幕引脚初始化
static void ST7735_Config(void);    // ST7735屏幕配置
static void ST7735_CLear(uint16_t);   // 清屏
static void SPI_ST7735_Write_Data(uint8_t);    // 向SPI总线传输一个字节数据
static void ST7735_Write_CMD(uint8_t);  // 向液晶屏写一个字节命令
static void ST7735_Write_DATA_8BIT(uint8_t);    // 向液晶屏写1个字节数据
static void ST7735_Write_DATA_16BIT(uint16_t); //  向液晶屏写2个字节数据 
static void ST7735_Reset(void);  // 屏幕复位
static void ST7735_Set_Region(uint16_t,uint16_t,uint16_t,uint16_t); // 设置lcd显示区域，在此区域写点数据自动换行
static void ST7735_Set_XY(uint16_t,uint16_t);   // 设置lcd显示起始点


static void GUI_ST7735_DrawPoint(uint16_t,uint16_t,uint16_t);  // 画一个点
static void GUI_ST7735_DrawLine(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t); // 画线
static void GUI_DrawFont_GBK16(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);   // 显示16x16 ASCII或汉字
static void GUI_DrawFont_GBK24(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *);    // 显示24x24 ASCII或汉字
static void GUI_DrawFont_Num32(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);   // 显示数码管数字及符号

#if NONE_USE_CODE
static void GUI_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);   // 在指定区域内填充单个颜色
static void GUI_ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);   // 显示图片(水平扫描，16位真彩色)
static void GUI_ST7735_Circle(uint16_t,uint16_t,uint16_t,uint16_t);    // 画圆
static uint16_t ST7735_GRB_To_RGB(uint16_t);  // GBR格式转RGB格式
static void GUI_ST7735_DrawBox(uint16_t, uint16_t, uint16_t, uint16_t,uint16_t); // 画盒子
#endif

/*====================================static function declaration area   END====================================*/
ST7735_t ST7735 = 
{
    ST7735_Init,
    ST7735_CLear,
    GUI_DrawFont_GBK16,
    GUI_DrawFont_GBK24,
    GUI_DrawFont_Num32,
};

/*
* @function: ST7735_Init
* @param: None
* @retval: None
* @brief: ST7735屏幕总初始化
*/
static void ST7735_Init(void)
{
    ST7735_GPIO_Init(); // 管脚初始化
    ST7735_Config();    // 屏幕配置
    ST7735.ST7735_CLear(LCD_BLUE);  //清屏
    GUI_DrawFont_GBK24(27,10,LCD_BLACK,LCD_BLUE,"嵌入式");
}

/*
* @function: ST7735_GPIO_Init
* @param: None
* @retval: None
* @brief: ST7735屏幕引脚初始化
*/
static void ST7735_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitStructure.Pins = ST7735_SCL_Pin | ST7735_SDA_Pin | ST7735_DC_Pin | ST7735_CS_Pin;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = ST7735_RES_Pin;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
}

/*
 * @function: ST7735_Config
 * @param: None
 * @retval: None
 * @brief: ST7735屏幕配置
 */
static void ST7735_Config(void)
{
    ST7735_Reset(); // Reset before LCD Init.

    // LCD Init For 1.44Inch LCD Panel with ST7735R.
    ST7735_Write_CMD(0x11); // Sleep exit
    Public.System_MS_Delay(120);

    // ST7735R Frame Rate
    ST7735_Write_CMD(0xB1);
    ST7735_Write_DATA_8BIT(0x01);
    ST7735_Write_DATA_8BIT(0x2C);
    ST7735_Write_DATA_8BIT(0x2D);

    ST7735_Write_CMD(0xB2);
    ST7735_Write_DATA_8BIT(0x01);
    ST7735_Write_DATA_8BIT(0x2C);
    ST7735_Write_DATA_8BIT(0x2D);

    ST7735_Write_CMD(0xB3);
    ST7735_Write_DATA_8BIT(0x01);
    ST7735_Write_DATA_8BIT(0x2C);
    ST7735_Write_DATA_8BIT(0x2D);
    ST7735_Write_DATA_8BIT(0x01);
    ST7735_Write_DATA_8BIT(0x2C);
    ST7735_Write_DATA_8BIT(0x2D);

    ST7735_Write_CMD(0xB4); // Column inversion
    ST7735_Write_DATA_8BIT(0x07);

    // ST7735R Power Sequence
    ST7735_Write_CMD(0xC0);
    ST7735_Write_DATA_8BIT(0xA2);
    ST7735_Write_DATA_8BIT(0x02);
    ST7735_Write_DATA_8BIT(0x84);
    ST7735_Write_CMD(0xC1);
    ST7735_Write_DATA_8BIT(0xC5);

    ST7735_Write_CMD(0xC2);
    ST7735_Write_DATA_8BIT(0x0A);
    ST7735_Write_DATA_8BIT(0x00);

    ST7735_Write_CMD(0xC3);
    ST7735_Write_DATA_8BIT(0x8A);
    ST7735_Write_DATA_8BIT(0x2A);
    ST7735_Write_CMD(0xC4);
    ST7735_Write_DATA_8BIT(0x8A);
    ST7735_Write_DATA_8BIT(0xEE);

    ST7735_Write_CMD(0xC5); // VCOM
    ST7735_Write_DATA_8BIT(0x0E);

    ST7735_Write_CMD(0x36); // MX, MY, RGB mode
    ST7735_Write_DATA_8BIT(0xC0);   // 重要！！！方向 C0/00/A0/60,  C8/08/A8/68

    // ST7735R Gamma Sequence
    ST7735_Write_CMD(0xe0);
    ST7735_Write_DATA_8BIT(0x0f);
    ST7735_Write_DATA_8BIT(0x1a);
    ST7735_Write_DATA_8BIT(0x0f);
    ST7735_Write_DATA_8BIT(0x18);
    ST7735_Write_DATA_8BIT(0x2f);
    ST7735_Write_DATA_8BIT(0x28);
    ST7735_Write_DATA_8BIT(0x20);
    ST7735_Write_DATA_8BIT(0x22);
    ST7735_Write_DATA_8BIT(0x1f);
    ST7735_Write_DATA_8BIT(0x1b);
    ST7735_Write_DATA_8BIT(0x23);
    ST7735_Write_DATA_8BIT(0x37);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x07);
    ST7735_Write_DATA_8BIT(0x02);
    ST7735_Write_DATA_8BIT(0x10);

    ST7735_Write_CMD(0xe1);
    ST7735_Write_DATA_8BIT(0x0f);
    ST7735_Write_DATA_8BIT(0x1b);
    ST7735_Write_DATA_8BIT(0x0f);
    ST7735_Write_DATA_8BIT(0x17);
    ST7735_Write_DATA_8BIT(0x33);
    ST7735_Write_DATA_8BIT(0x2c);
    ST7735_Write_DATA_8BIT(0x29);
    ST7735_Write_DATA_8BIT(0x2e);
    ST7735_Write_DATA_8BIT(0x30);
    ST7735_Write_DATA_8BIT(0x30);
    ST7735_Write_DATA_8BIT(0x39);
    ST7735_Write_DATA_8BIT(0x3f);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x07);
    ST7735_Write_DATA_8BIT(0x03);
    ST7735_Write_DATA_8BIT(0x10);

    ST7735_Write_CMD(0x2a);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x7f);

    ST7735_Write_CMD(0x2b);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(0x9f);

    ST7735_Write_CMD(0xF0); // Enable test command
    ST7735_Write_DATA_8BIT(0x01);
    ST7735_Write_CMD(0xF6); // Disable ram power save mode
    ST7735_Write_DATA_8BIT(0x00);

    ST7735_Write_CMD(0x3A); // 65k mode
    ST7735_Write_DATA_8BIT(0x05);

    ST7735_Write_CMD(0x29); // Display on
}

/*============================底层============================*/

/*
* @function: SPI_ST7735_Write_Data
* @param: Data -> 一个字节数据
* @retval: None
* @brief: 向SPI总线传输一个字节数据
*/
static void SPI_ST7735_Write_Data(uint8_t Data)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        if (Data & BIT7)
        {
            LCD_SDA_SET;    // 输出数据
        }
        else
        {
            LCD_SDA_CLR;    
        }
        LCD_SCL_CLR;
        LCD_SCL_SET;
        Data <<= 1;
    }
}

/*
* @function: ST7735_Write_CMD
* @param: CMD -> 待写入命令
* @retval: None
* @brief: 向液晶屏写一个字节命令
*/
static void ST7735_Write_CMD(uint8_t CMD)
{
    // CS拉低通信开始，CS拉高通信结束
    LCD_CS_CLR;
    LCD_DC_CLR;
    SPI_ST7735_Write_Data(CMD);
    LCD_CS_SET;
}

/*
* @function: ST7735_Write_DATA_8BIT
* @param: DATA -> 待写入数据
* @retval: None
* @brief: 向液晶屏写1个字节数据
*/
static void ST7735_Write_DATA_8BIT(uint8_t DATA)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI_ST7735_Write_Data(DATA);
    LCD_CS_SET;    
}

/*
* @function: ST7735_Write_DATA_16BIT
* @param: DATA -> 待写入数据
* @retval: None
* @brief: 向液晶屏写2个字节数据
*/
static void ST7735_Write_DATA_16BIT(uint16_t DATA)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI_ST7735_Write_Data(DATA >> 8);   // 先高再低
    SPI_ST7735_Write_Data(DATA);
    LCD_CS_SET;    
}

/*
* @function: ST7735_Reset
* @param: None
* @retval: None
* @brief: 屏幕复位
*/
static void ST7735_Reset(void)
{
    LCD_RES_CLR;
    Public.System_MS_Delay(100);
    LCD_RES_SET;
    Public.System_MS_Delay(50);
}

/*
* @function: ST7735_Set_Region
* @param: x_start -> 横坐标起点 y_start -> 纵坐标起点 x_end -> 横坐标结束点 y_end -> 纵坐标结束点
* @retval: None
* @brief: 设置lcd显示区域，在此区域写点数据自动换行
*/
static void ST7735_Set_Region(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    ST7735_Write_CMD(0x2a);
    ST7735_Write_DATA_8BIT(0x00);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
    ST7735_Write_DATA_8BIT(x_start);    // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
    ST7735_Write_DATA_8BIT(0x00);   // 起始位置y高位
    ST7735_Write_DATA_8BIT(x_end);  // 起始位置x低位

    ST7735_Write_CMD(0x2b);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(y_start + 0);
    ST7735_Write_DATA_8BIT(0x00);
    ST7735_Write_DATA_8BIT(y_end);

    ST7735_Write_CMD(0x2c);
}

/*
* @function: ST7735_Set_XY
* @param: None
* @retval: None
* @brief: 设置lcd显示起始点
*/
static void ST7735_Set_XY(uint16_t x,uint16_t y)
{
    ST7735_Set_Region(x,y,x,y);
}

/*
* @function: GUI_ST7735_DrawPoint
* @param: 竖屏---X -> 行 Y -> 列 fc -> 圆的颜色
* @retval: None
* @brief: 画一个点
*/
static void GUI_ST7735_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
    ST7735_Set_Region(x,y,x + 1,y + 1);
    ST7735_Write_DATA_16BIT(Data);
}

/*
* @function: ST7735_CLear
* @param: None
* @retval: None
* @brief: 清屏
*/
static void ST7735_CLear(uint16_t Color)
{
    uint16_t i, m;
    ST7735_Set_Region(0, 0, LCD_X_MAX - 1, LCD_Y_MAX - 1);
    ST7735_Write_CMD(0x2C);
    for (i = 0; i < LCD_X_MAX; i++)
    {
        for (m = 0; m < LCD_Y_MAX; m++)
        {
            ST7735_Write_DATA_16BIT(Color);
        }
    }
}

/*
* @function: GUI_ST7735_DrawLine
* @param: 竖屏---x0 -> 行起始坐标 y0 -> 列起始坐标 x1 -> 行终止坐标 y1 -> 列终止坐标 Color -> 线颜色
* @retval: None
* @brief: 画线 Bresenham算法
*/
static __attribute__((unused)) void GUI_ST7735_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Color)
{
    int dx,  // difference in x's
        dy,  // difference in y's
        dx2, // dx,dy * 2
        dy2,
        x_inc, // amount in pixel space to move during drawing
        y_inc, // amount in pixel space to move during drawing
        error, // the discriminant i.e. error i.e. decision variable
        index; // used for looping

    ST7735_Set_XY(x0, y0);
    dx = x1 - x0; // 计算x距离
    dy = y1 - y0; // 计算y距离

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) // x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {            // 且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) // 要画的点数不会超过x距离
        {
            // 画点
            GUI_ST7735_DrawPoint(x0, y0, Color);

            // test if error has overflowed
            if (error >= 0) // 是否需要增加y坐标值
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; // 增加y坐标值
            }                // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; // x坐标值每次画点后都递增1
        }                // end for
    }                    // end if |slope| <= 1
    else                 // y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {                    // 以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            GUI_ST7735_DrawPoint(x0, y0, Color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        } // end for
    }     // end else |slope| > 1
}


/*
* @function: GUI_DrawFont_GBK16
* @param: 竖屏---x -> 列 y -> 行 fc -> 字体颜色 bc -> 字体背景颜色 p_Str -> 字符串
* @retval: None
* @brief: 显示16x16字体
*/
static void GUI_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t* p_Str)
{
    uint8_t i, j;
    uint16_t k, x0;
    x0 = x;

    while(*p_Str)
    {
        if((*p_Str) < 128)  // 不是汉字
        {
            k = *p_Str;
            if (k == 13)
            {
                x = x0;
                y += 16;
            }
            else
            {
                if (k > 32) k -= 32;
                else k = 0;

                for(i = 0; i < 16; i++)
                    for(j = 0; j < 8; j++)
                    {
                        if(asc16[k * 16 + i] & (0x80 >> j))	
                        {
                            GUI_ST7735_DrawPoint(x + j, y + i, fc);
                        }
                        else
                        {
                            if (fc != bc) GUI_ST7735_DrawPoint(x + j, y + i, bc);
                        }
                    }
                x += 8;
            }
            p_Str++;
        }

        else    // 汉字
        {
            for (k = 0; k < hz16_num; k++)
            {
                if ((hz16[k].Index[0] == *(p_Str)) && (hz16[k].Index[1] == *(p_Str + 1)))
                {
                    for(i = 0; i < 16; i++)
                    {
                        for(j = 0; j < 8; j++)
                        {
                            if(hz16[k].Msk[i * 2] & (0x80 >> j))
                            {
                                GUI_ST7735_DrawPoint(x + j, y + i, fc);
                            }
                            else
                            {
                                if (fc != bc) GUI_ST7735_DrawPoint(x + j, y + i, bc);
                            }
                        }
                        for(j = 0; j < 8; j++)
                        {
                            if(hz16[k].Msk[i * 2 + 1] & (0x80 >> j))	
                            {
                                GUI_ST7735_DrawPoint(x + j + 8, y + i, fc);
                            }
                            else
                            {
                                if (fc != bc) GUI_ST7735_DrawPoint(x + j + 8, y + i, bc);
                            }
                        }
                    }
                }
            }
            p_Str += 2;
            x += 16;
        }
    }
}

/*
* @function: GUI_DrawFont_GBK24
* @param: 竖屏---x -> 列 y -> 行 fc -> 字体颜色 bc -> 字体背景颜色 p_Str -> 字符串
* @retval: None
* @brief: 显示24x24字体
*/
static void GUI_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *p_Str)
{
    uint8_t i, j;
    uint16_t k;

    while (*p_Str)
    {
        if (*p_Str < 0x80)
        {
            k = *p_Str;
            if (k > 32)
                k -= 32;
            else
                k = 0;

            for (i = 0; i < 16; i++)
                for (j = 0; j < 8; j++)
                {
                    if (asc16[k * 16 + i] & (0x80 >> j))
                    {
                        GUI_ST7735_DrawPoint(x + j, y + i, fc);
                    }
                    else
                    {
                        if (fc != bc)
                            GUI_ST7735_DrawPoint(x + j, y + i, bc);
                    }
                }
            p_Str++;
            x += 8;
        }
        else
        {
            for (k = 0; k < hz24_num; k++)
            {
                if ((hz24[k].Index[0] == *(p_Str)) && (hz24[k].Index[1] == *(p_Str + 1)))
                {
                    for (i = 0; i < 24; i++)
                    {
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3] & (0x80 >> j))
                            {
                                GUI_ST7735_DrawPoint(x + j, y + i, fc);
                            }
                            else
                            {
                                if (fc != bc)
                                {
                                    GUI_ST7735_DrawPoint(x + j, y + i, bc);
                                }
                            }
                        }
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3 + 1] & (0x80 >> j))
                            {
                                GUI_ST7735_DrawPoint(x + j + 8, y + i, fc);
                            }
                            else
                            {
                                if (fc != bc)
                                    GUI_ST7735_DrawPoint(x + j + 8, y + i, bc);
                            }
                        }
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3 + 2] & (0x80 >> j))
                            {
                                GUI_ST7735_DrawPoint(x + j + 16, y + i, fc);
                            }
                            else
                            {
                                if (fc != bc)
                                    GUI_ST7735_DrawPoint(x + j + 16, y + i, bc);
                            }
                        }
                    }
                }
            }
            p_Str += 2;
            x += 24;
        }
    }
}

/*
* @function: GUI_DrawFont_Num32
* @param: 竖屏---x -> 列 y -> 行 fc -> 字体颜色 bc -> 字体背景颜色 num -> sz32数组下标即可
* @retval: None
* @brief: 显示数码管数字及符号
*/
static void GUI_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
    uint8_t i, j, k, c;

    for(i = 0; i < 32; i++)
    {
        for(j = 0; j < 4; j++)
        {
            c = *(sz32 + num * 32 * 4 + i * 4 + j);
            for (k = 0; k < 8; k++)
            {

                if(c & (0x80 >> k))	
                {
                    GUI_ST7735_DrawPoint(x + j * 8 + k, y + i, fc);
                }
                else
                {
                    if (fc != bc) 
                    {
                        GUI_ST7735_DrawPoint(x + j * 8 + k, y + i, bc);
                    }
                }
            }
        }
    }
}



#if NONE_USE_CODE

/*
* @function: GUI_Fill
* @param: xStart -> 左上角起始x坐标 yStart -> 左上角起始y坐标 xEnd -> 右下角起始x坐标 yEnd -> 右下角起始y坐标 color-> 颜色
* @retval: None
* @brief: 在指定区域内填充单个颜色
*/
static void GUI_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    uint32_t pixel = (xEnd - xStart + 1) * (yEnd - yStart + 1); // 填充的像素数量

    ST7735_Set_Region(xStart, yStart, xEnd, yEnd);          // 设定填充范围
    while (pixel-- > 0)                             // 发送颜色值
        ST7735_Write_DATA_16BIT(color);
}

/*
* @function: GUI_DrawFont_Num32
* @param: 竖屏---x -> 列 y -> 行 width -> 图片宽度 height -> 图片高度 image -> 图片数组
* @retval: None
* @brief: 显示图片(水平扫描，16位真彩色)
*/
static void GUI_ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image)
{
    uint16_t colorData = 0;
    uint32_t cnt = 0;

    for (uint16_t i = 0; i < height; i++)           // 一行一行地显示
    {
        ST7735_Set_Region(x, y + i, x + width, y + height); // 重新设置光标位置
        for (uint16_t j = 0; j < width; j++)        // 一行中，从左到事，逐个像素处理
        {
            colorData = (image[cnt * 2 + 1] << 8) | image[cnt * 2];
            ST7735_Write_DATA_16BIT(colorData);              // 写入16位颜色数据
            cnt++;
        }
    }
}

/*
* @function: ST7735_GRB_To_RGB
* @param: c -> GBR格式的颜色值
* @retval: RGB格式的颜色值
* @brief: GBR格式转RGB格式
*/
static uint16_t ST7735_GRB_To_RGB(uint16_t GRB_Value)
{
    uint16_t r, g, b, rgb;

    b = (GRB_Value >> 0) & 0x1f;
    g = (GRB_Value >> 5) & 0x3f;
    r = (GRB_Value >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);

    return (rgb);
}

/*
* @function: GUI_ST7735_Circle
* @param: 竖屏---X -> 行 Y -> 列 R -> 半径 fc -> 圆的颜色
* @retval: None
* @brief: 画圆 Bresenham算法
*/
static void GUI_ST7735_Circle(uint16_t X,uint16_t Y,uint16_t R,uint16_t fc)
{
    uint16_t a, b;
    int c;
    a = 0;
    b = R;
    c = 3 - 2 * R;
    while (a < b)
    {
        GUI_ST7735_DrawPoint(X + a, Y + b, fc); //        7
        GUI_ST7735_DrawPoint(X - a, Y + b, fc); //        6
        GUI_ST7735_DrawPoint(X + a, Y - b, fc); //        2
        GUI_ST7735_DrawPoint(X - a, Y - b, fc); //        3
        GUI_ST7735_DrawPoint(X + b, Y + a, fc); //        8
        GUI_ST7735_DrawPoint(X - b, Y + a, fc); //        5
        GUI_ST7735_DrawPoint(X + b, Y - a, fc); //        1
        GUI_ST7735_DrawPoint(X - b, Y - a, fc); //        4

        if (c < 0)
            c = c + 4 * a + 6;
        else
        {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b)
    {
        GUI_ST7735_DrawPoint(X + a, Y + b, fc);
        GUI_ST7735_DrawPoint(X + a, Y + b, fc);
        GUI_ST7735_DrawPoint(X + a, Y - b, fc);
        GUI_ST7735_DrawPoint(X - a, Y - b, fc);
        GUI_ST7735_DrawPoint(X + b, Y + a, fc);
        GUI_ST7735_DrawPoint(X - b, Y + a, fc);
        GUI_ST7735_DrawPoint(X + b, Y - a, fc);
        GUI_ST7735_DrawPoint(X - b, Y - a, fc);
    }
}

/*
* @function: ST7735_CLear
* @param: 竖屏---x -> 列 y -> 行 w -> 宽 h -> 高 bc -> 对角线颜色
* @retval: None
* @brief: 画盒子
*/
static void GUI_ST7735_DrawBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t bc)
{
	GUI_ST7735_DrawLine(x, y, x+w, y, LCD_GRAY0);
	GUI_ST7735_DrawLine(x+w-1, y+1, x+w-1, y+1+h, LCD_GREEN);
	GUI_ST7735_DrawLine(x, y+h, x+w, y+h, LCD_YELLOW);
	GUI_ST7735_DrawLine(x, y, x, y+h, LCD_WHITE);
    GUI_ST7735_DrawLine(x+1, y+1, x+1+w-2, y+1+h-2, bc);
}
#endif
