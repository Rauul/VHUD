#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include <d3dx9.h>

#if _WIN64
#define FUEL_ICON				"Bin64\\Plugins\\VHUD\\Images\\Fuel.png"
#else
#define FUEL_ICON				"Bin32\\Plugins\\VHUD\\Images\\Fuel.png"
#endif

class Fuel
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	bool NewLapStarted(const TelemInfoV01& info);
	bool NewLapStarted(const ScoringInfoV01& info);
	void setPlayerSlot(const ScoringInfoV01& info);
	void Update(const TelemInfoV01& info);
	void Update(const ScoringInfoV01 & info);
	void UpdatePosition();
	void ResetFuelUsage();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawIcon();
	void DrawTxt();
		
	RECT size = { 0, 0, 220, 65 };
	D3DXVECTOR3 position = { 0, 600, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { BIG_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXSPRITE iconSprite = NULL;
	LPDIRECT3DTEXTURE9 iconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	int smallFontSize = 17;
	int bigFontSize = 40;
	int playerSlot = 0;
	int oldLap = 0;
	double quantity = 0.00;
	double quantityLastLap = 0.00;
	double usedPerLap[3] = { 0, 0, 0 };
	double bestLapLastSession = 0;
	double lastLapStartET = 0.00;
	double lapQuantity = 0.00;
	int timeQuantityTotal = 0;
	int timeQuantityMinutes = 0;
	int timeQuantitySeconds = 0;
	bool firstUpdate = true;
	bool useBorder = true;
	bool enabled = true;

private:

};