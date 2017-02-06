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
	/*clutch = info.mFilteredClutch;
	brake = info.mFilteredBrake;
	throttle = info.mFilteredThrottle;*/

	clutch = info.mUnfilteredClutch;
	brake = info.mUnfilteredBrake;
	throttle = info.mUnfilteredThrottle;
}

void Inputs::UpdatePosition()
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

void Inputs::DrawBackground()
{
	RECT size = { 0, 0, 64, 64 };
	D3DCOLOR color = 0xFFFFFFFF;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(backgroundTexture, &size, NULL, &position, color);
	iconSprite->End();
}

void Inputs::DrawBars()
{
	RECT clutchSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 clutchPosition = position;
	RECT brakeSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 brakePosition = position;
	RECT throttleSize = { 0, 0, 12, 0 };
	D3DXVECTOR3 throttlePosition = position;

	clutchPosition.x += 7;
	clutchPosition.y += 59 - 52 * clutch;
	brakePosition.x += 27;
	brakePosition.y += 59 - 52 * brake;
	throttlePosition.x += 47;
	throttlePosition.y += 59 - 52 * throttle;
	clutchSize.top -= 52 * clutch;
	brakeSize.top -= 52 * brake;
	throttleSize.top -= 52 * throttle;

	D3DCOLOR color = 0xFF284ad5;
	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(barTexture, &clutchSize, NULL, &clutchPosition, color);
	iconSprite->End();

	color = 0xFFc81515;
	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(barTexture, &brakeSize, NULL, &brakePosition, color);
	iconSprite->End();

	color = 0xFF2fca05;
	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(barTexture, &throttleSize, NULL, &throttlePosition, color);
	iconSprite->End();
}

void Inputs::DrawForeground()
{
	RECT size = { 0, 0, 62, 4 };
	D3DXVECTOR3 pos = position;

	pos.x += 1;
	pos.y += 9;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(boxTexture, &size, NULL, &pos, backgroundColor);
	iconSprite->End();
}
