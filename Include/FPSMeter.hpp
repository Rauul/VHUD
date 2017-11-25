#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include <d3dx9.h>

#if _WIN64
#define FPS_ICON				"Bin64\\Plugins\\VHUD\\Images\\FPS.png"
#else
#define FPS_ICON				"Bin32\\Plugins\\VHUD\\Images\\FPS.png"
#endif

class FPSMeter
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update();
	void UpdatePosition();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawTxt();
	long RoundNum(long num, long multiple);

	RECT size = { 0, 0, 64, 32 };
	D3DXVECTOR3 position = { 0, 950, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { BIG_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	DWORD startTime = GetTickCount();
	int fps = 0;
	int frames = 0;
	int warning = 60;
	bool useBorder = true;
	bool enabled = true;
};