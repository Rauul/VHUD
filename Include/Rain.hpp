#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include <d3dx9.h>

#if _WIN64
#define RAIN_ICON				"Bin64\\Plugins\\VHUD\\Images\\Rain.png"
#else
#define RAIN_ICON				"Bin32\\Plugins\\VHUD\\Images\\Rain.png"
#endif

class Rain
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update(const ScoringInfoV01& info);
	void UpdatePosition();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawIcon();
	void DrawTxt();
	long RoundNum(long num, long multiple);

	RECT size = { 0, 0, 170, 65 };
	D3DXVECTOR3 position = { 0, 810, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	D3DCOLOR iconColor = 0xFFFFFFFF;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { BIG_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXSPRITE iconSprite = NULL;
	LPDIRECT3DTEXTURE9 iconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	DWORD startTime = GetTickCount();
	DWORD interval = 3000;
	double rainPtc = 0;
	double onPathWetness = 0;
	double offPathWetness = 0;
	double trackTemp = 0;
	double ambientTemp = 0;
	int mode = 1;
	bool useBorder = true;
	bool enabled = true;
	bool showWetness = true;

private:

};