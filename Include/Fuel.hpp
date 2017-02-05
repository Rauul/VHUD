#pragma once

#include "InternalsPlugin.hpp"
#include <d3dx9.h>

#if _WIN64
#define BACKGROUND_TEXTURE		"Bin64\\Plugins\\VHUD\\Background.png"
#define FUEL_ICON				"Bin64\\Plugins\\VHUD\\Icons\\Fuel.png"
#else
#define BACKGROUND_TEXTURE		"Bin32\\Plugins\\VHUD\\Background.png"
#define FUEL_ICON				"Bin32\\Plugins\\VHUD\\Icons\\Fuel.png"
#endif

#define TAHOMA						"Tahoma"
#define BIG_FONT_SIZE				40
#define SMALL_FONT_SIZE				18

class Fuel
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update(const TelemInfoV01& info);
	void Draw();
	void DrawBox();
	void DrawIcon();
	void DrawTxt();

	Fuel(int x);

	RECT size;
	D3DXVECTOR3 position;
	D3DCOLOR backgroundColor, boarderColor;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { BIG_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, TAHOMA };
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, TAHOMA };
	LPD3DXSPRITE iconSprite = NULL;
	LPDIRECT3DTEXTURE9 iconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	double quantity = 0.00;
	double usedPerLap[3];
	double lapTime[3];
	bool useBoarder;
	bool enabled = true;

private:

};