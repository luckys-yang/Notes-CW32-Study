/***************************************************************************
 * File: USER
 * Author: Luckys.
 * Date: 2023/06/13
 * description: ST7735 LCD��
 -----------------------------------
���ߣ�
    ��Ƭ��GND   ----  ��ĻGND
    ��Ƭ��3.3V  ----  ��ĻVCC
    ��Ƭ��PB6   ----  ��ĻSCL(MISO)
    ��Ƭ��PB7   ----  ��ĻSDA(MOSI)
    ��Ƭ��PA15  ----  ��ĻRES
    ��Ƭ��PB4   ----  ��ĻDC
    ��Ƭ��PB5   ----  ��ĻCS(Ƭѡ)
    ��Ƭ��3.3V  ----  ��ĻBLK     
ע�⣺
    ʹ��TFT��Ļ��Ҫ�� public.h �򿪡�USE_TFT�� ��        
 -----------------------------------
****************************************************************************/
#include "main.h"
#include "lcd_font.h"

/*====================================static function declaration area BEGIN====================================*/

static void ST7735_Init(void);  // ST7735��Ļ�ܳ�ʼ��
static void ST7735_GPIO_Init(void); // ST7735��Ļ���ų�ʼ��
static void ST7735_Config(void);    // ST7735��Ļ����
static void ST7735_CLear(uint16_t);   // ����
static void SPI_ST7735_Write_Data(uint8_t);    // ��SPI���ߴ���һ���ֽ�����
static void ST7735_Write_CMD(uint8_t);  // ��Һ����дһ���ֽ�����
static void ST7735_Write_DATA_8BIT(uint8_t);    // ��Һ����д1���ֽ�����
static void ST7735_Write_DATA_16BIT(uint16_t); //  ��Һ����д2���ֽ����� 
static void ST7735_Reset(void);  // ��Ļ��λ
static void ST7735_Set_Region(uint16_t,uint16_t,uint16_t,uint16_t); // ����lcd��ʾ�����ڴ�����д�������Զ�����
static void ST7735_Set_XY(uint16_t,uint16_t);   // ����lcd��ʾ��ʼ��


static void GUI_ST7735_DrawPoint(uint16_t,uint16_t,uint16_t);  // ��һ����
static void GUI_ST7735_DrawLine(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t); // ����
static void GUI_DrawFont_GBK16(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);   // ��ʾ16x16 ASCII����
static void GUI_DrawFont_GBK24(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *);    // ��ʾ24x24 ASCII����
static void GUI_DrawFont_Num32(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);   // ��ʾ��������ּ�����

#if NONE_USE_CODE
static void GUI_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);   // ��ָ����������䵥����ɫ
static void GUI_ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);   // ��ʾͼƬ(ˮƽɨ�裬16λ���ɫ)
static void GUI_ST7735_Circle(uint16_t,uint16_t,uint16_t,uint16_t);    // ��Բ
static uint16_t ST7735_GRB_To_RGB(uint16_t);  // GBR��ʽתRGB��ʽ
static void GUI_ST7735_DrawBox(uint16_t, uint16_t, uint16_t, uint16_t,uint16_t); // ������
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
* @brief: ST7735��Ļ�ܳ�ʼ��
*/
static void ST7735_Init(void)
{
    ST7735_GPIO_Init(); // �ܽų�ʼ��
    ST7735_Config();    // ��Ļ����
    ST7735.ST7735_CLear(LCD_BLUE);  //����
    GUI_DrawFont_GBK24(27,10,LCD_BLACK,LCD_BLUE,"Ƕ��ʽ");
}

/*
* @function: ST7735_GPIO_Init
* @param: None
* @retval: None
* @brief: ST7735��Ļ���ų�ʼ��
*/
static void ST7735_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // �������
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
 * @brief: ST7735��Ļ����
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
    ST7735_Write_DATA_8BIT(0xC0);   // ��Ҫ���������� C0/00/A0/60,  C8/08/A8/68

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

/*============================�ײ�============================*/

/*
* @function: SPI_ST7735_Write_Data
* @param: Data -> һ���ֽ�����
* @retval: None
* @brief: ��SPI���ߴ���һ���ֽ�����
*/
static void SPI_ST7735_Write_Data(uint8_t Data)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        if (Data & BIT7)
        {
            LCD_SDA_SET;    // �������
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
* @param: CMD -> ��д������
* @retval: None
* @brief: ��Һ����дһ���ֽ�����
*/
static void ST7735_Write_CMD(uint8_t CMD)
{
    // CS����ͨ�ſ�ʼ��CS����ͨ�Ž���
    LCD_CS_CLR;
    LCD_DC_CLR;
    SPI_ST7735_Write_Data(CMD);
    LCD_CS_SET;
}

/*
* @function: ST7735_Write_DATA_8BIT
* @param: DATA -> ��д������
* @retval: None
* @brief: ��Һ����д1���ֽ�����
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
* @param: DATA -> ��д������
* @retval: None
* @brief: ��Һ����д2���ֽ�����
*/
static void ST7735_Write_DATA_16BIT(uint16_t DATA)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI_ST7735_Write_Data(DATA >> 8);   // �ȸ��ٵ�
    SPI_ST7735_Write_Data(DATA);
    LCD_CS_SET;    
}

/*
* @function: ST7735_Reset
* @param: None
* @retval: None
* @brief: ��Ļ��λ
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
* @param: x_start -> ��������� y_start -> ��������� x_end -> ����������� y_end -> �����������
* @retval: None
* @brief: ����lcd��ʾ�����ڴ�����д�������Զ�����
*/
static void ST7735_Set_Region(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    ST7735_Write_CMD(0x2a);
    ST7735_Write_DATA_8BIT(0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
    ST7735_Write_DATA_8BIT(x_start);    // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
    ST7735_Write_DATA_8BIT(0x00);   // ��ʼλ��y��λ
    ST7735_Write_DATA_8BIT(x_end);  // ��ʼλ��x��λ

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
* @brief: ����lcd��ʾ��ʼ��
*/
static void ST7735_Set_XY(uint16_t x,uint16_t y)
{
    ST7735_Set_Region(x,y,x,y);
}

/*
* @function: GUI_ST7735_DrawPoint
* @param: ����---X -> �� Y -> �� fc -> Բ����ɫ
* @retval: None
* @brief: ��һ����
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
* @brief: ����
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
* @param: ����---x0 -> ����ʼ���� y0 -> ����ʼ���� x1 -> ����ֹ���� y1 -> ����ֹ���� Color -> ����ɫ
* @retval: None
* @brief: ���� Bresenham�㷨
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
    dx = x1 - x0; // ����x����
    dy = y1 - y0; // ����y����

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

    if (dx > dy) // x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
    {            // ���ߵĵ�������x���룬��x���������
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) // Ҫ���ĵ������ᳬ��x����
        {
            // ����
            GUI_ST7735_DrawPoint(x0, y0, Color);

            // test if error has overflowed
            if (error >= 0) // �Ƿ���Ҫ����y����ֵ
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; // ����y����ֵ
            }                // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; // x����ֵÿ�λ���󶼵���1
        }                // end for
    }                    // end if |slope| <= 1
    else                 // y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
    {                    // ��y��Ϊ��������
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
* @param: ����---x -> �� y -> �� fc -> ������ɫ bc -> ���屳����ɫ p_Str -> �ַ���
* @retval: None
* @brief: ��ʾ16x16����
*/
static void GUI_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t* p_Str)
{
    uint8_t i, j;
    uint16_t k, x0;
    x0 = x;

    while(*p_Str)
    {
        if((*p_Str) < 128)  // ���Ǻ���
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

        else    // ����
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
* @param: ����---x -> �� y -> �� fc -> ������ɫ bc -> ���屳����ɫ p_Str -> �ַ���
* @retval: None
* @brief: ��ʾ24x24����
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
* @param: ����---x -> �� y -> �� fc -> ������ɫ bc -> ���屳����ɫ num -> sz32�����±꼴��
* @retval: None
* @brief: ��ʾ��������ּ�����
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
* @param: xStart -> ���Ͻ���ʼx���� yStart -> ���Ͻ���ʼy���� xEnd -> ���½���ʼx���� yEnd -> ���½���ʼy���� color-> ��ɫ
* @retval: None
* @brief: ��ָ����������䵥����ɫ
*/
static void GUI_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    uint32_t pixel = (xEnd - xStart + 1) * (yEnd - yStart + 1); // ������������

    ST7735_Set_Region(xStart, yStart, xEnd, yEnd);          // �趨��䷶Χ
    while (pixel-- > 0)                             // ������ɫֵ
        ST7735_Write_DATA_16BIT(color);
}

/*
* @function: GUI_DrawFont_Num32
* @param: ����---x -> �� y -> �� width -> ͼƬ��� height -> ͼƬ�߶� image -> ͼƬ����
* @retval: None
* @brief: ��ʾͼƬ(ˮƽɨ�裬16λ���ɫ)
*/
static void GUI_ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image)
{
    uint16_t colorData = 0;
    uint32_t cnt = 0;

    for (uint16_t i = 0; i < height; i++)           // һ��һ�е���ʾ
    {
        ST7735_Set_Region(x, y + i, x + width, y + height); // �������ù��λ��
        for (uint16_t j = 0; j < width; j++)        // һ���У������£�������ش���
        {
            colorData = (image[cnt * 2 + 1] << 8) | image[cnt * 2];
            ST7735_Write_DATA_16BIT(colorData);              // д��16λ��ɫ����
            cnt++;
        }
    }
}

/*
* @function: ST7735_GRB_To_RGB
* @param: c -> GBR��ʽ����ɫֵ
* @retval: RGB��ʽ����ɫֵ
* @brief: GBR��ʽתRGB��ʽ
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
* @param: ����---X -> �� Y -> �� R -> �뾶 fc -> Բ����ɫ
* @retval: None
* @brief: ��Բ Bresenham�㷨
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
* @param: ����---x -> �� y -> �� w -> �� h -> �� bc -> �Խ�����ɫ
* @retval: None
* @brief: ������
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
