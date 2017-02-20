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

bool Fuel::NewLapStarted(const ScoringInfoV01 & info)
{
	if (oldLap != info.mVehicle[playerSlot].mTotalLaps)
	{
		oldLap = info.mVehicle[playerSlot].mTotalLaps;
		return true;
	}

	return false;
}

void Fuel::setPlayerSlot(const ScoringInfoV01 & info)
{
	if (playerSlot < 0 || !info.mVehicle[playerSlot].mIsPlayer)
	{
		for (long i = 0; i < info.mNumVehicles; i++)
		{
			if (info.mVehicle[i].mIsPlayer)
			{
				playerSlot = i;
				break;
			}
		}
	}
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
		if (quantityLastLap - quantity > info.mFuelCapacity * 0.01)
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

	int laps = 0;
	double total = 0;
	for (int i = 0; i < 3; i++)
	{
		if (usedPerLap[i] > 0)
		{
			total += usedPerLap[i];
			laps++;
		}
		else
			break;
	}

	if (laps > 0)
	{
		double avgFuelConsumption = total / laps;
		lapQuantity = quantity / avgFuelConsumption;
	}
}

void Fuel::Update(const ScoringInfoV01 & info)
{
	setPlayerSlot(info);
	timeQuantityTotal = 0;

	if (NewLapStarted(info))
	{
		if ((info.mVehicle[playerSlot].mBestLapTime < bestLapLastSession && info.mVehicle[playerSlot].mBestLapTime > 0) || bestLapLastSession < 1)
			bestLapLastSession = info.mVehicle[playerSlot].mBestLapTime;
	}

	int laps = 0;
	double total = 0;
	for (int i = 0; i < 3; i++)
	{
		if (usedPerLap[i] > 0)
		{
			total += usedPerLap[i];
			laps++;
		}
		else
			break;
	}

	if (laps > 0 && (info.mVehicle[playerSlot].mBestLapTime > 0 || bestLapLastSession > 0))
	{
		double avgFuelConsumption = total / laps;

		if(info.mVehicle[playerSlot].mBestLapTime > 0)
			timeQuantityTotal = quantity / (avgFuelConsumption / info.mVehicle[playerSlot].mBestLapTime);
		else
			timeQuantityTotal = quantity / (avgFuelConsumption / bestLapLastSession);
	}

	timeQuantityMinutes = timeQuantityTotal / 60;
	timeQuantitySeconds = timeQuantityTotal % 60;
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
	bestLapLastSession = lapQuantity = 0;
	usedPerLap[0] = usedPerLap[1] = usedPerLap[2] = 0;
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

	if (usedPerLap[0] > 0 && lapQuantity < 3)
		color = 0xFFFF8000;

	if (usedPerLap[0] > 0 && lapQuantity < 1)
		color = 0xFFFF0000;

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
	pos.right = pos.left + size.right - 50;
	pos.bottom = pos.top + 36;
	char text[32] = "";

	sprintf(text, "%.2f", quantity);
	bigFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);

	pos.top += 40;
	pos.bottom = pos.top + 24;

	sprintf(text, "%.2f | %.1f | %02d:%02d", usedPerLap[0], lapQuantity, timeQuantityMinutes, timeQuantitySeconds);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
