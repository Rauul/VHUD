#include "Fuel.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Fuel::Init(const ScreenInfoV01 & info)
{
	GetPrivateProfileString("Config", "Font", "Tahoma", smallFontDesc.FaceName, 32, CONFIG_FILE);
	smallFontDesc.Height = GetPrivateProfileInt("Config", "SmallFontSize", 20, CONFIG_FILE);
	GetPrivateProfileString("Config", "Font", "Tahoma", bigFontDesc.FaceName, 32, CONFIG_FILE);
	bigFontDesc.Height = GetPrivateProfileInt("Config", "BigFontSize", 40, CONFIG_FILE);

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
	fuelCapacity = info.mFuelCapacity;

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

	// Calculate time
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

	//Calculate fuel needed to finish
	fuelNeededToFinish = 0;
	fullTanksNeeded = 0;
	if (laps > 0 && (info.mVehicle[playerSlot].mBestLapTime > 0 || bestLapLastSession > 0))
	{
		double avgFuelConsumption = total / laps;

		if (info.mMaxLaps > 999999)
			if (info.mVehicle[playerSlot].mBestLapTime > 0)
				fuelNeededToFinish = (avgFuelConsumption / info.mVehicle[playerSlot].mBestLapTime * (info.mEndET - info.mCurrentET)) - quantity + avgFuelConsumption;
			else
				fuelNeededToFinish = (avgFuelConsumption / bestLapLastSession * (info.mEndET - info.mCurrentET)) - quantity + avgFuelConsumption;
		else
			fuelNeededToFinish = (info.mMaxLaps - (info.mVehicle[playerSlot].mTotalLaps + (info.mVehicle[playerSlot].mLapDist / info.mLapDist))) * avgFuelConsumption - quantity;

		fullTanksNeeded = fuelNeededToFinish / fuelCapacity;
		fuelNeededToFinish = fmod(fuelNeededToFinish, fuelCapacity);
	}

	/*if (showFullTanks)
	{
		if (fuelNeededToFinish < 0)
			fuelNeededToFinish = 0;
		if (fuelNeededToFinish > fuelCapacity)
			fuelNeededToFinish = fuelCapacity;
	}*/
}

void Fuel::UpdatePosition()
{
	if (!enabled)
		return;

	POINT cursorPosition;
	if (GetCursorPos(&cursorPosition))
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			position.x = cursorPosition.x - size.right / 2;
			position.y = cursorPosition.y - size.bottom / 2;
		}

		else if (GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			position.x = RoundNum(cursorPosition.x, 10) - size.right / 2;
			position.y = RoundNum(cursorPosition.y, 10) - size.bottom / 2;
		}
		else
		{
			position.x = RoundNum(cursorPosition.x, 5) - size.right / 2;
			position.y = RoundNum(cursorPosition.y, 5) - size.bottom / 2;
		}
	}
}

long Fuel::RoundNum(long num, long multiple)
{
	long rem = num % multiple;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
}

void Fuel::ResetFuelUsage()
{
	bestLapLastSession = lapQuantity = fuelNeededToFinish = 0;
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

	if(fullTanksNeeded > 0)
		sprintf(text, "%.2f | %.1f | %02d:%02d | (%d)%.0f", usedPerLap[0], lapQuantity, timeQuantityMinutes, timeQuantitySeconds, fullTanksNeeded, fuelNeededToFinish);
	else
		sprintf(text, "%.2f | %.1f | %02d:%02d | %.0f", usedPerLap[0], lapQuantity, timeQuantityMinutes, timeQuantitySeconds, fuelNeededToFinish);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
