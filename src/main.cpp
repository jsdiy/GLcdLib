//	グラフィックLCDライブラリー
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/1	初版

#include <Arduino.h>
#include "ImgFBlue.hpp"		//50x50「F」マーク
#include "LcdILI9225.hpp"
#include "Img20240210V.hpp"	//縦_551HORAI
#include "Img20250330H.hpp"	//横_池のある公園
#include "Img20251124V.hpp"	//縦_紅葉した樹木
#include "LcdST77xx.hpp"
#include "Img128x160V.hpp"	//縦_夜の駅前

//オブジェクト
LcdILI9225	lcd;
//LcdST77xx	lcd;

//色
static	Color	ColorWhite	= Color(0xFF, 0xFF, 0xFF);
static	Color	ColorBlack	= Color(0x00, 0x00, 0x00);
static	Color	ColorRed	= Color(0xFF, 0x00, 0x00);
static	Color	ColorGreen	= Color(0x00, 0xFF, 0x00);
static	Color	ColorBlue	= Color(0x00, 0x00, 0xFF);
static	Color	ColorYellow	= Color(0xFF, 0xFF, 0x00);

void	setup()
{
	Serial.begin(115200);
	delay(1000);

	//初期化
	//lcd.Initialize(GPIO_NUM_10, GPIO_NUM_4, 7735, 128, 160, 0, 0);	//ST7735
	lcd.Initialize(GPIO_NUM_10, GPIO_NUM_4, 1, 1, 1);	//ILI9225
	lcd.ClearScreen(ColorBlue);

	lcd.SetScale(1, 2);
	lcd.DrawString(0, 0, "Hello, World.");
	delay(1000);

	//lcd.RotateFlip(ERotFlip::Rot180);
	//lcd.DrawImage(0, 0, Img128x160V::Width, Img128x160V::Height, Img128x160V::Datas, Img128x160V::Length);
	
	//lcd.RotateFlip(ERotFlip::Rot90);
	lcd.DrawImage(0, 0, Img20240210V::Width, Img20240210V::Height, Img20240210V::Datas, Img20240210V::Length);
}

void	loop()
{
	delay(1000);
}
