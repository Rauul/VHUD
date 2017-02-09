#pragma once

#include "InternalsPlugin.hpp"
#include <vector>
#include <d3dx9.h>

#if _WIN64
#define STARTINGLIGHTS_ICON			"Bin64\\Plugins\\VHUD\\Images\\StartingLights.png"
#define STARTINGLIGHT_OFF			"Bin64\\Plugins\\VHUD\\Images\\SL_off.png"
#define STARTINGLIGHT_RED			"Bin64\\Plugins\\VHUD\\Images\\SL_red.png"
#define STARTINGLIGHT_GREEN			"Bin64\\Plugins\\VHUD\\Images\\SL_green.png"
#define STARTINGLIGHT_YELLOW		"Bin64\\Plugins\\VHUD\\Images\\SL_yellow.png"
#else
#define STARTINGLIGHTS_ICON			"Bin32\\Plugins\\VHUD\\Images\\StartingLightsIcon.png"
#define STARTINGLIGHTS_TEXTURE		"Bin32\\Plugins\\VHUD\\Images\\StartingLights.png"
#endif

class StartingLights
{
public:
	void ResetLights();
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update(const ScoringInfoV01& info);
	void Update(const TelemInfoV01& info);
	void UpdatePosition();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawLights(bool inEditMode);

	RECT size = { 0, 0, 400, 90 };
	D3DXVECTOR3 position = { 0, 1020, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXSPRITE offLightSprite = NULL;
	LPDIRECT3DTEXTURE9 offLightTexture = NULL;
	LPD3DXSPRITE redLightSprite = NULL;
	LPDIRECT3DTEXTURE9 redLightTexture = NULL;
	LPD3DXSPRITE yellowLightSprite = NULL;
	LPDIRECT3DTEXTURE9 yellowLightTexture = NULL;
	LPD3DXSPRITE greenLightSprite = NULL;
	LPDIRECT3DTEXTURE9 greenLightTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	int numLights = 0;
	int numRedLights = 0;
	long ID = 0;
	bool inPits = false;
	bool showLights = false;
	bool numLightsSet = false;
	bool useBorder = true;
	bool enabled = true;

private:
};