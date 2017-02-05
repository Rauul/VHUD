#pragma once

#include <d3dx9.h>

#if _WIN64
#define BACKGROUND_TEXTURE			"Bin64\\Plugins\\VHUD\\Background.png"
#define BIG_FUEL_ICON				"Bin64\\Plugins\\VHUD\\Icons\\FuelBig.png"
#else
#define BACKGROUND_TEXTURE			"Bin32\\Plugins\\VHUD\\Background.png"
#define BIG_FUEL_ICON				"Bin32\\Plugins\\VHUD\\Icons\\FuelBig.png"
#endif

class Menu
{
public:
	Menu(int x);
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Draw();
	void DrawBox();
	void DrawIcon(int slot, LPD3DXSPRITE& sprite, LPDIRECT3DTEXTURE9& texture);

	RECT size;
	D3DXVECTOR3 position;
	D3DCOLOR backgroundColor, boarderColor;
	LPD3DXSPRITE fuelIconSprite = NULL;
	LPDIRECT3DTEXTURE9 fuelIconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	bool useBoarder;
};