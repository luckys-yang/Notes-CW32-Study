/***************************************************************************
 * File: menu.c
 * Author: Luckys.
 * Date: 2023/06/15
 * description: OLED显示界面
****************************************************************************/
#include "main.h"

/*====================================static function declaration area BEGIN====================================*/

static void Menu_Page1(void);   // Page1
static void Menu_Page2(void);   // Page2
static void Menu_Page3(void);   // Page3
static void Menu_Page4(void);   // Page4

/*====================================static function declaration area   END====================================*/

Page_Display_t Page1 = {" "};
Page_Display_t Page2 = {" "};
Page_Display_t Page3 = {" "};
Page_Display_t Page4 = {" "};

Menu_t Menu = 
{
    PAGE_TempHum,
    Menu_Page1,
    Menu_Page2,
    Menu_Page3,
    Menu_Page4,
};

/*
* @function: Menu_Page1
* @param: None
* @retval: None
* @brief: Page1
*/
static void Menu_Page1(void)
{
    OLED096.OLED096_Show_CHN(8,OLED096_Line1,"温");
    OLED096.OLED096_Show_CHN(32,OLED096_Line1,"湿");
    OLED096.OLED096_Show_CHN(56,OLED096_Line1,"度");
    OLED096.OLED096_Show_CHN(80,OLED096_Line1,"采");
    OLED096.OLED096_Show_CHN(104,OLED096_Line1,"集");

    OLED096.OLED096_Show_String(0, OLED096_Line2, (char*)Page1.OLED096_Display_Buff[0], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line4,  (char*)Page1.OLED096_Display_Buff[1], ASCII_SIZE_16);
    OLED096.OLED096_Show_CHN(80,OLED096_Line2,"℃");
}

/*
* @function: Menu_Page2
* @param: None
* @retval: None
* @brief: Page2
*/
static void Menu_Page2(void)
{
    OLED096.OLED096_Show_CHN(13,OLED096_Line1,"日");
    OLED096.OLED096_Show_CHN(37,OLED096_Line1,"期");
    OLED096.OLED096_Show_CHN(61,OLED096_Line1,"时");
    OLED096.OLED096_Show_CHN(85,OLED096_Line1,"间");

    OLED096.OLED096_Show_String(0, OLED096_Line2, (char*)Page2.OLED096_Display_Buff[0], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line3, (char*)Page2.OLED096_Display_Buff[1], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line4, (char*)Page2.OLED096_Display_Buff[2], ASCII_SIZE_16);
}

/*
* @function: Menu_Page3
* @param: None
* @retval: None
* @brief: Page3
*/
static void Menu_Page3(void)
{
    OLED096.OLED096_Show_CHN(13,OLED096_Line1,"电");
    OLED096.OLED096_Show_CHN(37,OLED096_Line1,"压");
    OLED096.OLED096_Show_CHN(61,OLED096_Line1,"采");
    OLED096.OLED096_Show_CHN(85,OLED096_Line1,"集");

    OLED096.OLED096_Show_String(0, OLED096_Line2, (char*)Page3.OLED096_Display_Buff[0], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line4,  (char*)Page3.OLED096_Display_Buff[1], ASCII_SIZE_16);
}

/*
* @function: Menu_Page4
* @param: None
* @retval: None
* @brief: Page4
*/
static void Menu_Page4(void)
{
    OLED096.OLED096_Show_CHN(13,OLED096_Line1,"脉");
    OLED096.OLED096_Show_CHN(37,OLED096_Line1,"冲");
    OLED096.OLED096_Show_CHN(61,OLED096_Line1,"普");
    OLED096.OLED096_Show_CHN(85,OLED096_Line1,"通");

    OLED096.OLED096_Show_String(0, OLED096_Line2, (char*)Page4.OLED096_Display_Buff[0], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line3,  (char*)Page4.OLED096_Display_Buff[1], ASCII_SIZE_16);
    OLED096.OLED096_Show_String(0, OLED096_Line4,  (char*)Page4.OLED096_Display_Buff[2], ASCII_SIZE_16);
}
