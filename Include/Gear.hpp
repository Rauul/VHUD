#pragma once
#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include <d3dx9.h>
#include <Windows.h>

#if _WIN64
#define GEAR_ICON				"Bin64\\Plugins\\VHUD\\Images\\Gear.png"
#else
#define GEAR_ICON				"Bin32\\Plugins\\VHUD\\Images\\Gear.png"
#endif

class Gear
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
	void DrawTxt();

	RECT size = { 0, 0, 64, 65 };
	D3DXVECTOR3 position = { 0, 950, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { GEAR_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, BIG_FONT };
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	char gear[4] = "-";
	bool shiftLight = false;
	bool useBorder = true;
	bool enabled = true;
};