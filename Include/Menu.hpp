#pragma once

#include <d3dx9.h>

class Menu
{
public:
	Menu(int x);
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Draw(bool inEditMode);
	void DrawBox();
	void DrawIcon(int slot, LPD3DXSPRITE& sprite, LPDIRECT3DTEXTURE9& texture);

	RECT size;
	D3DXVECTOR3 position;
	D3DCOLOR backgroundColor, iconColor, iconMouseOverColor, boarderColor;
	LPD3DXSPRITE fuelIconSprite = NULL;
	LPDIRECT3DTEXTURE9 fuelIconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	int mouseInSlot = -1;
	bool useBoarder;
	long screenWidth;
};