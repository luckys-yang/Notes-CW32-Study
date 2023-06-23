/***************************************************************************
 * File: oled_096.c
 * Author: Luckys.
 * Date: 2023/06/13
 * description: 0.96寸OLED
 -----------------------------------
接线：
    VCC -----> 3.3V
    GND -----> GND
    SCL -----> PB6
    SDA -----> PB7
 -----------------------------------
****************************************************************************/
#include "main.h"
#include "oled_font.h"

/*====================================static function declaration area BEGIN====================================*/

static void OLED096_Init(void);     // OLED初始化
static void OLED096_Clear(void);    // 清屏
static void OLED096_Show_String(uint8_t, OLED091_Line_t, const char*, ASCII_Size_t);       // OLED显示字符串
static void OLED096_Show_CHN(uint8_t, OLED091_Line_t, const char*);        // OLED显示单个汉字
static void padString(char *, int);

static void OLED096_Set_Pos(uint8_t, uint8_t); // OLED设置坐标
static void OLED096_Write_CMD(uint8_t);    // OLED写命令
static void OLED096_Write_Data(uint8_t);    // OLED写数据

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
* @param: CMD -> 待写入命令
* @retval: None
* @brief: OLED写命令
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
* @param: Data -> 待写入数据
* @retval: None
* @brief: OLED写数据
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
* @brief: OLED初始化
*/
static void OLED096_Init(void)
{
    Public.System_MS_Delay(200); // 上电延时
    OLED096_Write_CMD(0xAE); // OLED休眠
    OLED096_Write_CMD(0x00); // 设置低列地址
    OLED096_Write_CMD(0x10); // 设置高列地址
    OLED096_Write_CMD(0x40); // 设置起始地址线
    OLED096_Write_CMD(0xB0); // set page address
    OLED096_Write_CMD(0x81); // 设置对比度
    OLED096_Write_CMD(0xFF); //--128
    OLED096_Write_CMD(0xA1); // 0xa0左右反置 0xa1正常
    OLED096_Write_CMD(0xA6); // normal / reverse
    OLED096_Write_CMD(0xA8); // 设置多路复用(1 to 64)
    OLED096_Write_CMD(0x3F); // 1/32 duty
    OLED096_Write_CMD(0xC8); // Com scan direction
    OLED096_Write_CMD(0xD3); // 设置显示的偏移映射内存计数器
    OLED096_Write_CMD(0x00); //

    OLED096_Write_CMD(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED096_Write_CMD(0x80); // 设置分频比例，时钟设置为100帧/秒

    OLED096_Write_CMD(0xD8); // set area color mode off
    OLED096_Write_CMD(0x05); //

    OLED096_Write_CMD(0xD9); // 预充电时间
    OLED096_Write_CMD(0xF1); // 预充电为15个脉冲，释放为1个脉冲

    OLED096_Write_CMD(0xDA); // 引脚设置硬件配置
    OLED096_Write_CMD(0x12); //

    OLED096_Write_CMD(0xDB); // 设置VCOM电平
    OLED096_Write_CMD(0x30); //
    // 唤醒
    OLED096_Write_CMD(0x8D); // 设置电荷泵
    OLED096_Write_CMD(0x14); // 开启电荷泵

    OLED096_Write_CMD(0xAF); // OLED唤醒(AE是OLED休眠)

    OLED096.OLED096_Clear();    // 清屏      
}

/*
* @function: OLED096_Clear
* @param: None
* @retval: None
* @brief: OLED清屏
*/
static void OLED096_Clear(void)
{
    uint8_t Page, Seg;

    for (Page = 0; Page < 8; Page++)
    {
        OLED096_Write_CMD(0xB0 + Page); // 一共8页(行)
        OLED096_Write_CMD(0x00);        // 低
        OLED096_Write_CMD(0x01);        // 高

        for (Seg = 0; Seg < 128; Seg++)
        {
            OLED096_Write_Data(0x00);
        }
    }
}

/*
* @function     : OLED096_Set_Pos
* @param        : Page -> 行 Seg -> 列
* @retval       : None
* @brief        : OLED设置坐标
*/
static void OLED096_Set_Pos(uint8_t Page, uint8_t Seg)
{
    OLED096_Write_CMD(0xB0 + Seg);
    OLED096_Write_CMD(((Page & 0xF0) >> 4) | 0x10); // 高4位
    OLED096_Write_CMD((Page & 0x0F));               // 低4位
}

/*
* @function: OLED096_Show_String
* @param: x -> 列 y -> 行 p_Str -> 要显示的字符串 ch_size -> 字体大小
* @retval: None
* @brief: OLED显示字符串
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
* @param: x -> 列 y -> 行  p_Str -> 单个汉字字符串
* @retval: None
* @brief: // OLED显示单个汉字
*/
static void OLED096_Show_CHN(uint8_t x, OLED091_Line_t y, const char *p_Str)
{
    uint16_t usCHN_Number; // 字库中汉字数量
    uint16_t usIndex;      // 字库中的汉字索引
    uint8_t i;

    // 统计汉字的位置
    usCHN_Number = sizeof(CHN_16x16) / sizeof(Oled_Font16x16_t);
    // 查找汉字的位置
    for (usIndex = 0; usIndex < usCHN_Number; usIndex++)
    {
        if ((CHN_16x16[usIndex].Index[0] == *p_Str) && (CHN_16x16[usIndex].Index[1] == *(p_Str + 1))) // 因为一个汉字占两个字节
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
* @param: str -> 字串符 size -> 最大不能超过多少
* @retval: None
* @brief: 补全空格
*/
static __inline void padString(char *str, int size)
{
    int len = strlen(str);
    if (len >= size)
    {
        return; // 字符串已经够长了，不需要添加空格
    }
    for (int i = len; i < size; i++)
    {
        if (i < size - 1)
        {
            str[i] = ' '; // 在字符串末尾添加空格
        }
        else
        {
            str[i] = '\0'; // 添加字符串结束符
        }
    }
}
