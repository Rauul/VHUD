#pragma once

#include "InternalsPlugin.hpp"
#include <d3dx9.h>

#if _WIN64
#define INPUTS_ICON				"Bin64\\Plugins\\VHUD\\Images\\InputsIcon.png"
#define INPUTS_BACKGROUND		"Bin64\\Plugins\\VHUD\\Images\\InputsBackground.png"
#define INPUTS_BAR				"Bin64\\Plugins\\VHUD\\Images\\InputsBar.png"
#else
#define INPUTS_ICON				"Bin32\\Plugins\\VHUD\\Images\\InputsIcon.png"
#define INPUTS_BACKGROUND		"Bin32\\Plugins\\VHUD\\Images\\InputsBackground.png"
#define INPUTS_BAR				"Bin32\\Plugins\\VHUD\\Images\\InputsBar.png"
#endif

class Inputs
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
	void DrawBackground();
	void DrawBars();
	void DrawForeground();
	long RoundNum(long num, long multiple);

	RECT size = { 0, 0, 64, 65 };
	D3DXVECTOR3 position = { 0, 880, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXSPRITE backgroundSprite = NULL;
	LPDIRECT3DTEXTURE9 backgroundTexture = NULL;
	LPD3DXSPRITE iconSprite = NULL;
	LPDIRECT3DTEXTURE9 iconTexture = NULL;
	LPD3DXSPRITE barSprite = NULL;
	LPDIRECT3DTEXTURE9 barTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	double unfilteredClutch = 0;
	double unfilteredBrake = 0;
	double unfilteredThrottle = 0;
	double filteredClutch = 0;
	double filteredBrake = 0;
	double filteredThrottle = 0;
	bool showFilteredInputs = false;
	bool useBorder = true;
	bool enabled = true;	
};