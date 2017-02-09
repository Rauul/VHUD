#pragma once

#include "InternalsPlugin.hpp"
#include <d3dx9.h>

#if _WIN64
#define MOUSE_TEXTURE			"Bin64\\Plugins\\VHUD\\Images\\Cursor.png"
#else
#define MOUSE_TEXTURE			"Bin32\\Plugins\\VHUD\\Images\\Cursor.png"
#endif

class Cursor
{
public:
	Cursor() {}
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Draw(bool inEditMode);

	RECT size = { 0, 0, 32, 32 };
	D3DXVECTOR3 position = { 0, 0, 0, };
	D3DCOLOR color = 0xFFFFFFFF;
	LPD3DXSPRITE cursorSprite = NULL;
	LPDIRECT3DTEXTURE9 cursorTexture = NULL;

	enum LockedTo
	{
		None,
		Fuel,
		Tyres,
		Engine,
		Rain,
		Inputs,
		FPSMeter,
		StartingLights
	};

	LockedTo lockedTo = LockedTo::None;
};