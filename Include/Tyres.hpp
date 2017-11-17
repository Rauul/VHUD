#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include <d3dx9.h>

#if _WIN64
#define TYRES_ICON				"Bin64\\Plugins\\VHUD\\Images\\Tyres.png"
#else
#define TYRES_ICON				"Bin32\\Plugins\\VHUD\\Images\\Tyres.png"
#endif

class Tyres
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update(const TelemInfoV01& info);
	void UpdatePosition();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawIcon();
	void DrawTxt();
	long RoundNum(long num, long multiple);

	RECT size = { 0, 0, 170, 65 };
	D3DXVECTOR3 position = { 0, 670, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXSPRITE iconSprite = NULL;
	LPDIRECT3DTEXTURE9 iconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	DWORD startTime = GetTickCount();
	DWORD interval = 3000;
	double wearFL = 0.00;
	double wearFR = 0.00;
	double wearRL = 0.00;
	double wearRR = 0.00;
	double tempFL = 0.00;
	double tempFR = 0.00;
	double tempRL = 0.00;
	double tempRR = 0.00;
	double threshold = 125;
	int mode = 1;
	bool useBorder = true;
	bool enabled = true;
	bool showWear = true;

private:
};