#include "Inputs.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Inputs::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &iconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &backgroundSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &barSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, RAIN_ICON, &iconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, INPUTS_BACKGROUND, &backgroundTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, INPUTS_BAR, &barTexture);
}

void Inputs::Uninit(const ScreenInfoV01 & info)
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
	if (backgroundTexture) {
		backgroundTexture->Release();
		backgroundTexture = NULL;
	}
	if (backgroundSprite) {
		backgroundSprite->Release();
		backgroundSprite = NULL;
	}
	if (barTexture) {
		barTexture->Release();
		barTexture = NULL;
	}
	if (barSprite) {
		barSprite->Release();
		barSprite = NULL;
	}
}

void Inputs::PreReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (backgroundSprite)
		backgroundSprite->OnLostDevice();
	if (barSprite)
		barSprite->OnLostDevice();
}

void Inputs::PostReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (backgroundSprite)
		backgroundSprite->OnResetDevice();
	if (barSprite)
		barSprite->OnResetDevice();
}

void Inputs::Update(const TelemInfoV01 & info)
{
	filteredClutch = info.mFilteredClutch;
	filteredBrake = info.mFilteredBrake;
	filteredThrottle = info.mFilteredThrottle;

	unfilteredClutch = info.mUnfilteredClutch;
	unfilteredBrake = info.mUnfilteredBrake;
	unfilteredThrottle = info.mUnfilteredThrottle;
}

void Inputs::UpdatePosition()
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

long Inputs::RoundNum(long num, long multiple)
{
	long rem = num % multiple;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
}

void Inputs::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawBackground();
	DrawBars();
	DrawForeground();
}

void Inputs::DrawBox(bool inEditMode)
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

void Inputs::DrawBackground()
{
	RECT size = { 0, 0, 64, 64 };
	D3DCOLOR color = 0xFFFFFFFF;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(backgroundTexture, &size, NULL, &position, backgroundColor);
	iconSprite->End();
}

void Inputs::DrawBars()
{
	RECT unfilteredClutchSize = { 0, 0, 12, 0 }, filteredClutchSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 unfilteredClutchPosition = position, filteredClutchPosition = position;
	RECT unfilteredBrakeSize = { 0, 0, 12, 0 }, filteredBrakeSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 unfilteredBrakePosition = position, filteredBrakePosition = position;
	RECT unfilteredThrottleSize = { 0, 0, 12, 0 }, filteredThrottleSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 unfilteredThrottlePosition = position, filteredThrottlePosition = position;
	D3DCOLOR color;

	unfilteredClutchPosition.x += 6;
	unfilteredClutchPosition.y += 58 - 52 * unfilteredClutch;
	unfilteredBrakePosition.x += 26;
	unfilteredBrakePosition.y += 58 - 52 * unfilteredBrake;
	unfilteredThrottlePosition.x += 46;
	unfilteredThrottlePosition.y += 58 - 52 * unfilteredThrottle;
	unfilteredClutchSize.top -= 52 * unfilteredClutch;
	unfilteredBrakeSize.top -= 52 * unfilteredBrake;
	unfilteredThrottleSize.top -= 52 * unfilteredThrottle;

	filteredClutchPosition.x += 6;
	filteredClutchPosition.y += 58 - 52 * filteredClutch;
	filteredBrakePosition.x += 26;
	filteredBrakePosition.y += 58 - 52 * filteredBrake;
	filteredThrottlePosition.x += 46;
	filteredThrottlePosition.y += 58 - 52 * filteredThrottle;
	filteredClutchSize.top -= 52 * filteredClutch;
	filteredBrakeSize.top -= 52 * filteredBrake;
	filteredThrottleSize.top -= 52 * filteredThrottle;

	if (showFilteredInputs)
	{
		color = 0xAA366bd5;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &filteredClutchSize, NULL, &filteredClutchPosition, color);
		iconSprite->End();
		color = 0xFF366bd5; //284ad5;
		iconSprite->Begin(D3DXSPRITE_SORT_TEXTURE);
		iconSprite->Draw(barTexture, &unfilteredClutchSize, NULL, &unfilteredClutchPosition, color);
		iconSprite->End();

		color = 0x80c81515;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &unfilteredBrakeSize, NULL, &unfilteredBrakePosition, color);
		iconSprite->End();
		color = 0xFFc81515;
		iconSprite->Begin(D3DXSPRITE_SORT_TEXTURE);
		iconSprite->Draw(barTexture, &filteredBrakeSize, NULL, &filteredBrakePosition, color);
		iconSprite->End();

		color = 0x802fca05;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &unfilteredThrottleSize, NULL, &unfilteredThrottlePosition, color);
		iconSprite->End();
		color = 0xFF2fca05;
		iconSprite->Begin(D3DXSPRITE_SORT_TEXTURE);
		iconSprite->Draw(barTexture, &filteredThrottleSize, NULL, &filteredThrottlePosition, color);
		iconSprite->End();
	}

	else
	{
		color = 0xFF366bd5;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &unfilteredClutchSize, NULL, &unfilteredClutchPosition, color);
		iconSprite->End();

		color = 0xFFc81515;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &unfilteredBrakeSize, NULL, &unfilteredBrakePosition, color);
		iconSprite->End();

		color = 0xFF2fca05;
		iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconSprite->Draw(barTexture, &unfilteredThrottleSize, NULL, &unfilteredThrottlePosition, color);
		iconSprite->End();
	}
}

void Inputs::DrawForeground()
{
	RECT size = { 0, 0, 12, 4 };
	D3DXVECTOR3 pos = position;

	pos.x += 6;
	pos.y += 9;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	for (int i = 0; i < 3; i++)
	{
		iconSprite->Draw(boxTexture, &size, NULL, &pos, backgroundColor);
		pos.x += 20;
	}
	iconSprite->End();
}
