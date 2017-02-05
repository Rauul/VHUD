#include "Fuel.hpp"
#include "VHUD.hpp"
#include <stdio.h>

Fuel::Fuel(int x)
{
	position.x = 200;
	position.y = 200;
	position.z = 0;
	size.left = 0;
	size.top = 0;
	size.right = 218;
	size.bottom = 65;
	backgroundColor = 0xFF000000;
	boarderColor = 0xFFAAAAAA;
	useBoarder = true;
}

void Fuel::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &iconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, FUEL_ICON, &iconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);
}

void Fuel::Uninit(const ScreenInfoV01 & info)
{
	if (iconTexture) {
		iconTexture->Release();
		iconTexture = NULL;
	}
	if (iconSprite) {
		iconSprite->Release();
		iconSprite = NULL;
	}
	if (boxTexture) {
		boxTexture->Release();
		boxTexture = NULL;
	}
	if (boxSprite) {
		boxSprite->Release();
		boxSprite = NULL;
	}
	if (bigFont) {
		bigFont->Release();
		bigFont = NULL;
	}
	if (smallFont) {
		smallFont->Release();
		smallFont = NULL;
	}
}

void Fuel::PreReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (bigFont)
		bigFont->OnLostDevice();
	if (smallFont)
		smallFont->OnLostDevice();
}

void Fuel::PostReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (bigFont)
		bigFont->OnResetDevice();
	if (smallFont)
		smallFont->OnResetDevice();
}

void Fuel::Update(const TelemInfoV01& info)
{
	quantity = info.mFuel;
}

void Fuel::Draw()
{
	if (!enabled)
		return;

	DrawBox();
	DrawIcon();
	DrawTxt();
}

void Fuel::DrawBox()
{
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	boxSprite->Draw(boxTexture, &size, NULL, &position, backgroundColor);
	boxSprite->End();

	if (useBoarder)
	{
		RECT borderSize = size;
		D3DXVECTOR3 borderPosition = position;

		// Top
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.bottom = 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();

		// Bottom
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.y += size.bottom - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();

		// Left
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.right = 1;
		borderSize.bottom = size.bottom - 2;
		borderPosition.y = position.y + 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();

		// Right
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.x += size.right - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();
	}
}

void Fuel::DrawIcon()
{
	RECT sizee = { 0, 0, 64, 64 };
	D3DXVECTOR3 pos = { 200, 200, 0 };
	D3DCOLOR color = 0xFFFFFFFF;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(iconTexture, &sizee, NULL, &pos, color);
	iconSprite->End();
}

void Fuel::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x + 64;
	pos.top = position.y + 4;
	pos.right = pos.left + 154;
	pos.bottom = pos.top + 36;
	char text[32] = "";

	sprintf(text, "%.2f", quantity);
	bigFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);

	pos.top += 36;
	pos.bottom = pos.top + 24;

	sprintf(text, "%.2f", 2.61);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
