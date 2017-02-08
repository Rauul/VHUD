#include "Fuel.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Fuel::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &iconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, FUEL_ICON, &iconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);

	position.x = info.mWidth / 2 - size.right / 2;
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

bool Fuel::NewLapStarted(const TelemInfoV01 & info)
{
	if (info.mLapStartET != lastLapStartET)
		return true;

	return false;
}

void Fuel::Update(const TelemInfoV01& info)
{
	quantity = info.mFuel;

	if (firstUpdate)
	{
		quantityLastLap = quantity;
		firstUpdate = false;
	}

	if (NewLapStarted(info))
	{
		if (quantityLastLap - quantity > 0.1)
		{
			for (int i = 2; i > 0; i--)
			{
				usedPerLap[i] = usedPerLap[i - 1];
			}
			usedPerLap[0] = quantityLastLap - quantity;
		}
		quantityLastLap = quantity;
		lastLapStartET = info.mLapStartET;
	}

	if (usedPerLap[0] > 0 && usedPerLap[1] > 0 && usedPerLap[2] > 0)
	{
		double avgFuelConsumption = (usedPerLap[0] + usedPerLap[1] + usedPerLap[2]) / 3;
		lapQuantity = quantity / avgFuelConsumption;
	}
}

void Fuel::UpdatePosition()
{
	if (!enabled)
		return;

	POINT cursorPosition;
	if (GetCursorPos(&cursorPosition))
	{
		position.x = cursorPosition.x - size.right / 2;
		position.y = cursorPosition.y - size.bottom / 2;
	}
}

void Fuel::ResetFuelUsage()
{
	for each (double d in usedPerLap)
	{
		d = 0;
	}
}

void Fuel::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawIcon();
	DrawTxt();
}

void Fuel::DrawBox(bool inEditMode)
{
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	boxSprite->Draw(boxTexture, &size, NULL, &position, backgroundColor);
	boxSprite->End();

	if (useBorder || inEditMode)
	{
		RECT borderSize = size;
		D3DXVECTOR3 borderPosition = position;

		// Top
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.bottom = 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Bottom
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.y += size.bottom - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Left
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.right = 1;
		borderSize.bottom = size.bottom - 2;
		borderPosition.y = position.y + 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Right
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.x += size.right - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();
	}
}

void Fuel::DrawIcon()
{
	RECT size = { 0, 0, 64, 64 };
	D3DCOLOR color = 0xFFFFFFFF;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(iconTexture, &size, NULL, &position, color);
	iconSprite->End();
}

void Fuel::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x + 50;
	pos.top = position.y + 2;
	pos.right = pos.left + 121;
	pos.bottom = pos.top + 36;
	char text[32] = "";

	sprintf(text, "%.2f", quantity);
	bigFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);

	pos.top += 40;
	pos.bottom = pos.top + 24;

	sprintf(text, "%.2f | %.1f", usedPerLap[0], lapQuantity);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
