/***************************************************************************
 * File: oled_096.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 0.96��OLED
 -----------------------------------
���ߣ�
    VCC -----> 3.3V
    GND -----> GND
    SCL -----> PB6
    SDA -----> PB7
 -----------------------------------
****************************************************************************/
#include "main.h"
#include "oled_font.h"

/*====================================static function declaration area BEGIN====================================*/

static void OLED096_Init(void);     // OLED��ʼ��
static void OLED096_Clear(void);    // ����
static void OLED096_Show_String(uint8_t, OLED091_Line_t, const char*, ASCII_Size_t);       // OLED��ʾ�ַ���
static void OLED096_Show_CHN(uint8_t, OLED091_Line_t, const char*);        // OLED��ʾ��������
static void padString(char *, int);

static void OLED096_Set_Pos(uint8_t, uint8_t); // OLED��������
static void OLED096_Write_CMD(uint8_t);    // OLEDд����
static void OLED096_Write_Data(uint8_t);    // OLEDд����

/*====================================static function declaration area   END====================================*/
OLED096_t OLED096 =
{
    // {" "},
    padString,
    OLED096_Init,
    OLED096_Clear,
    OLED096_Show_String,
    OLED096_Show_CHN,
};

/*
* @function: OLED096_Write_CMD
* @param: CMD -> ��д������
* @retval: None
* @brief: OLEDд����
*/
static void OLED096_Write_CMD(uint8_t CMD)
{
    I2C_Soft.I2C_Start();
    I2C_Soft.I2C_Write_Byte(OLED096_ADDR);        // R/W#=0
    I2C_Soft.I2C_Write_Byte(0x00);       // Co=0,D/C#=0
    I2C_Soft.I2C_Write_Byte(CMD);
    I2C_Soft.I2C_Stop();    
}

/*
* @function: OLED096_Write_Data
* @param: Data -> ��д������
* @retval: None
* @brief: OLEDд����
*/
static void OLED096_Write_Data(uint8_t Data)
{
    I2C_Soft.I2C_Start();
    I2C_Soft.I2C_Write_Byte(OLED096_ADDR);        // R/W#=0
    I2C_Soft.I2C_Write_Byte(0x40);       // Co=0,D/C#=0
    I2C_Soft.I2C_Write_Byte(Data);
    I2C_Soft.I2C_Stop();    
}

/*
* @function: OLED096_Init
* @param: None
* @retval: None
* @brief: OLED��ʼ��
*/
static void OLED096_Init(void)
{
    Public.System_MS_Delay(200); // �ϵ���ʱ
    OLED096_Write_CMD(0xAE); // OLED����
    OLED096_Write_CMD(0x00); // ���õ��е�ַ
    OLED096_Write_CMD(0x10); // ���ø��е�ַ
    OLED096_Write_CMD(0x40); // ������ʼ��ַ��
    OLED096_Write_CMD(0xB0); // set page address
    OLED096_Write_CMD(0x81); // ���öԱȶ�
    OLED096_Write_CMD(0xFF); //--128
    OLED096_Write_CMD(0xA1); // 0xa0���ҷ��� 0xa1����
    OLED096_Write_CMD(0xA6); // normal / reverse
    OLED096_Write_CMD(0xA8); // ���ö�·����(1 to 64)
    OLED096_Write_CMD(0x3F); // 1/32 duty
    OLED096_Write_CMD(0xC8); // Com scan direction
    OLED096_Write_CMD(0xD3); // ������ʾ��ƫ��ӳ���ڴ������
    OLED096_Write_CMD(0x00); //

    OLED096_Write_CMD(0xD5); // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED096_Write_CMD(0x80); // ���÷�Ƶ������ʱ������Ϊ100֡/��

    OLED096_Write_CMD(0xD8); // set area color mode off
    OLED096_Write_CMD(0x05); //

    OLED096_Write_CMD(0xD9); // Ԥ���ʱ��
    OLED096_Write_CMD(0xF1); // Ԥ���Ϊ15�����壬�ͷ�Ϊ1������

    OLED096_Write_CMD(0xDA); // ��������Ӳ������
    OLED096_Write_CMD(0x12); //

    OLED096_Write_CMD(0xDB); // ����VCOM��ƽ
    OLED096_Write_CMD(0x30); //
    // ����
    OLED096_Write_CMD(0x8D); // ���õ�ɱ�
    OLED096_Write_CMD(0x14); // ������ɱ�

    OLED096_Write_CMD(0xAF); // OLED����(AE��OLED����)

    OLED096.OLED096_Clear();    // ����      
}

/*
* @function: OLED096_Clear
* @param: None
* @retval: None
* @brief: OLED����
*/
static void OLED096_Clear(void)
{
    uint8_t Page, Seg;

    for (Page = 0; Page < 8; Page++)
    {
        OLED096_Write_CMD(0xB0 + Page); // һ��8ҳ(��)
        OLED096_Write_CMD(0x00);        // ��
        OLED096_Write_CMD(0x01);        // ��

        for (Seg = 0; Seg < 128; Seg++)
        {
            OLED096_Write_Data(0x00);
        }
    }
}

/*
* @function     : OLED096_Set_Pos
* @param        : Page -> �� Seg -> ��
* @retval       : None
* @brief        : OLED��������
*/
static void OLED096_Set_Pos(uint8_t Page, uint8_t Seg)
{
    OLED096_Write_CMD(0xB0 + Seg);
    OLED096_Write_CMD(((Page & 0xF0) >> 4) | 0x10); // ��4λ
    OLED096_Write_CMD((Page & 0x0F));               // ��4λ
}

/*
* @function: OLED096_Show_String
* @param: x -> �� y -> �� p_Str -> Ҫ��ʾ���ַ��� ch_size -> �����С
* @retval: None
* @brief: OLED��ʾ�ַ���
*/
static void OLED096_Show_String(uint8_t x, OLED091_Line_t y, const char *p_Str, ASCII_Size_t ch_size)
{
    uint8_t i = 0;
    uint8_t c = 0;

    if (ch_size == 16)
    {
        while (p_Str[i] != '\0')
        {
            c = p_Str[i++] - ' ';
            OLED096_Set_Pos(x, y);
            for (uint8_t j = 0; j < 8; j++)
                OLED096_Write_Data(ucASCII_16x8[c * 16 + j]);
            OLED096_Set_Pos(x, y + 1);
            for (uint8_t j = 0; j < 8; j++)
                OLED096_Write_Data(ucASCII_16x8[c * 16 + j + 8]);

            x += 8;
            if (x > 120)
            {
                x = 0;
                y += 2;
            }
        }
    }
}

/*
* @function: OLED096_Show_CHN
* @param: x -> �� y -> ��  p_Str -> ���������ַ���
* @retval: None
* @brief: // OLED��ʾ��������
*/
static void OLED096_Show_CHN(uint8_t x, OLED091_Line_t y, const char *p_Str)
{
    uint16_t usCHN_Number; // �ֿ��к�������
    uint16_t usIndex;      // �ֿ��еĺ�������
    uint8_t i;

    // ͳ�ƺ��ֵ�λ��
    usCHN_Number = sizeof(CHN_16x16) / sizeof(Oled_Font16x16_t);
    // ���Һ��ֵ�λ��
    for (usIndex = 0; usIndex < usCHN_Number; usIndex++)
    {
        if ((CHN_16x16[usIndex].Index[0] == *p_Str) && (CHN_16x16[usIndex].Index[1] == *(p_Str + 1))) // ��Ϊһ������ռ�����ֽ�
        {
            OLED096_Set_Pos(x, y);
            for (i = 0; i < 16; i++)
            {
                OLED096_Write_Data(CHN_16x16[usIndex].CHN_code[i]);
            }

            OLED096_Set_Pos(x, y + 1);
            for (i = 0; i < 16; i++)
            {
                OLED096_Write_Data(CHN_16x16[usIndex].CHN_code[i + 16]);
            }
            break;
        }
    }
}

/*
* @function: Menu_Display
* @param: str -> �ִ��� size -> ����ܳ�������
* @retval: None
* @brief: ��ȫ�ո�
*/
static __inline void padString(char *str, int size)
{
    int len = strlen(str);
    if (len >= size)
    {
        return; // �ַ����Ѿ������ˣ�����Ҫ��ӿո�
    }
    for (int i = len; i < size; i++)
    {
        if (i < size - 1)
        {
            str[i] = ' '; // ���ַ���ĩβ��ӿո�
        }
        else
        {
            str[i] = '\0'; // ����ַ���������
        }
    }
}
