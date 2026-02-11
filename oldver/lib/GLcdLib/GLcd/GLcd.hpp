//	SPI-DMA対応グラフィックLCDライブラリー：基底クラス
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/01	初版

#pragma	once

#include <Arduino.h>
#include "Color.hpp"
#include "GLcdSpiDma.hpp"
#include "GLcdText.hpp"

//SPIピンアサイン
//ESP32のVSPI:		SS = GPIO_NUM_5,  MOSI = GPIO_NUM_23, (MISO = GPIO_NUM_19), SCK = GPIO_NUM_18
//ESP32-C3のSPI2:	SS = GPIO_NUM_10, MOSI = GPIO_NUM_7,  (MISO = GPIO_NUM_2),  SCK = GPIO_NUM_6

//反転/回転表示のパラメータ
//指定パターン:	Normal, RotXXX, FlipXXX, RotXXX|FlipXXX
enum	class	ERotFlip	: uint8_t
{
	Normal	= 0x00,	//LCDの正位置（多くの場合、LCDパネルのフレキシブルケーブルを下側とする向き）
	Rot90	= 0x01,	//LCDの正位置に対して画像を90度回転させる（画面座標系が回転する）
	Rot180	= 0x02,	//LCDの正位置に対して画像を180度回転させる（画面座標系が回転する）
	Rot270	= 0x04,	//LCDの正位置に対して画像を270度(-90度)回転させる（画面座標系が回転する）
	FlipHorizontal	= 0x08,	//画像の正位置に対して水平反転（画面座標系は反転しない）
	FlipVertical	= 0x10	//画像の正位置に対して垂直反転（画面座標系は反転しない）
};

//enumのビット演算子'|'(OR)のオーバーロード
//・キャストせず'A|B'と書ける。
constexpr	ERotFlip	operator |(ERotFlip lhs, ERotFlip rhs)
{
	return static_cast<ERotFlip>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

//グラフィックLCDクラス
class	GLcd	: public GLcdText
{
protected:
	GLcdSpiDma	glcdSpi;
	int16_t		LcdWidth, LcdHeight;	//LCDモジュールとしての画面サイズ（不変）
	int16_t		screenWidth, screenHeight;	//現在の画面サイズ（90度回転を考慮した論理的な画面サイズ）
	uint8_t*	grBuffer;	//画素データの汎用バッファ
	size_t		grBufferLength;
	virtual	void	SetGRamArea(int16_t x, int16_t y, int16_t w, int16_t h) = 0;	//描画領域を設定する
	virtual	void	SendCommandWriteGRam() = 0;	//GRAMに色データを書き込むコマンドを実行する

public:
	GLcd() : grBuffer(nullptr) {}
	int16_t	Width() { return screenWidth; }
	int16_t	Height() { return screenHeight; }
	void	Initialize(uint8_t spiMode, uint32_t spiClock, gpio_num_t pinSS, gpio_num_t pinDC, int16_t width, int16_t height);
	void	HwReset(gpio_num_t pinRESET);
	virtual	void	RotateFlip(ERotFlip param) = 0;	//ERotFlipに基づく回転・反転の実装
	void	SwapWidthHeight(bool doSwap);	//RotateFlip()に含めるべき縦横入れ替え関数（GLcdTextへの反映を含む）
	virtual	void	FillRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color);	//override可
	virtual	void	DrawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* imgData, size_t dataLength);	//override可
	void	ClearScreen(const Color& color) { FillRect(0, 0, screenWidth, screenHeight, color); }
	uint8_t*	GetLineBuffer(size_t* bufLength = nullptr);
};
