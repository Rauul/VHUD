#pragma once

#include <d3dx9.h>

class Menu
{
public:
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Draw(bool inEditMode);
	void DrawBox();
	void DrawIcon(int slot, LPD3DXSPRITE& sprite, LPDIRECT3DTEXTURE9& texture);

	RECT size = { 0, 0, 686, 74 };
	D3DXVECTOR3 position = { 200, 200, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR iconColor = 0xFFCCCCCC;
	D3DCOLOR iconMouseOverColor = 0xFFFFFFFF;
	D3DCOLOR boarderColor = 0xFFAAAAAA;
	LPD3DXSPRITE fuelIconSprite = NULL;
	LPDIRECT3DTEXTURE9 fuelIconTexture = NULL;
	LPD3DXSPRITE tyresIconSprite = NULL;
	LPDIRECT3DTEXTURE9 tyresIconTexture = NULL;
	LPD3DXSPRITE engineIconSprite = NULL;
	LPDIRECT3DTEXTURE9 engineIconTexture = NULL;
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	int mouseInSlot = -1;
	bool useBorder = true;
	long screenWidth;
};