//	SPI-DMA対応グラフィックLCDライブラリ：基底クラス
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/01	初版

#include <Arduino.h>
#include "GLcd.hpp"

//初期化
void	GLcd::Initialize(uint8_t spiMode, uint32_t spiClock, gpio_num_t pinSS, gpio_num_t pinDC, int16_t width, int16_t height)
{
	//画面サイズを決定する
	screenWidth = LcdWidth = width;
	screenHeight = LcdHeight = height;
	Serial.printf("LCD screen size: %d x %d\n", LcdWidth, LcdHeight);

	//ラインバッファ確保
	grBufferLength = std::max(LcdWidth, LcdHeight) * Color::Length;
	grBuffer = (uint8_t*)heap_caps_malloc(grBufferLength, MALLOC_CAP_DMA | MALLOC_CAP_32BIT);
	Serial.printf("LCD grBuffer size: %d bytes\n", grBufferLength);

	//テキスト描画機能を初期化する
	GLcdText::Initialize(screenWidth, screenHeight);

	//SPIバスにLCDモジュールを登録する
	bool isOK = glcdSpi.AddDevice(spiMode, spiClock, pinSS, pinDC);
	Serial.printf("SPI mode=%d, clock=%d, pinSS: %d, pinDC: %d, %s\n", spiMode, spiClock, pinSS, pinDC, isOK ? "OK" : "NG");
}

//ハードウェアリセット
void	GLcd::HwReset(gpio_num_t pinRESET)
{
	if (pinRESET == GPIO_NUM_NC) { return; }
	pinMode(pinRESET, OUTPUT);
	digitalWrite(pinRESET, LOW);	delay(100);
	digitalWrite(pinRESET, HIGH);	delay(300);
}

//回転状態に応じて画面の縦横サイズを入れ替える
void	GLcd::SwapWidthHeight(bool doSwap)
{
	if (doSwap) { screenWidth = LcdHeight;	screenHeight = LcdWidth; }
	else { screenWidth = LcdWidth;	screenHeight = LcdHeight; }
	GLcdText::SetTextScreen(screenWidth, screenHeight);
}

//単色で矩形領域を塗りつぶす
//引数	x,y,w,h:	画面上の四角形領域（画面に収まっていること）
void	GLcd::FillRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color)
{
	size_t dataLength = w * Color::Length;
	for (size_t i = 0; i < dataLength; i += Color::Length) { memcpy(&grBuffer[i], color.Bytes, Color::Length); }

	glcdSpi.BeginTransaction();
	SetGRamArea(x, y, w, h);
	SendCommandWriteGRam();
	for (int16_t i = 0; i < h; i++) { glcdSpi.SendData(grBuffer, dataLength); }
	glcdSpi.EndTransaction();
}

//画像を描く
//引数	x,y,w,h:	画面上の四角形領域（画面に収まっていること）
//		imgData:	画素データ（DMA対応メモリであること）
//		dataLength:	データ長(byte)
void	GLcd::DrawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* imgData, size_t dataLength)
{
	glcdSpi.BeginTransaction();
	SetGRamArea(x, y, w, h);
	SendCommandWriteGRam();
	glcdSpi.SendData(imgData, dataLength);
	glcdSpi.EndTransaction();
}

//DMA対応のラインバッファを外部へ提示する
//引数（戻り値）：	利用可能なバッファ長（必要に応じて返す）
//戻り値：	バッファのポインタ
uint8_t*	GLcd::GetLineBuffer(size_t* bufLength)
{
	if (bufLength != nullptr) { *bufLength = grBufferLength; }
	return grBuffer;
}
