#include "Rain.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Rain::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &iconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, RAIN_ICON, &iconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);

	position.x = info.mWidth / 2 - size.right / 2;
}

void Rain::Uninit(const ScreenInfoV01 & info)
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

void Rain::PreReset(const ScreenInfoV01 & info)
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

void Rain::PostReset(const ScreenInfoV01 & info)
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

void Rain::Update(const ScoringInfoV01& info)
{
	rainPtc = info.mRaining * 100;
	onPathWetness = info.mMinPathWetness * 100;
	offPathWetness = info.mMaxPathWetness * 100;
}

void Rain::UpdatePosition()
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

void Rain::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawIcon();
	DrawTxt();
}

void Rain::DrawBox(bool inEditMode)
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

void Rain::DrawIcon()
{
	RECT size = { 0, 0, 64, 64 };
	D3DCOLOR color = 0xFFFFFFFF;

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(iconTexture, &size, NULL, &position, color);
	iconSprite->End();
}

void Rain::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x + 50;
	pos.top = position.y + 2;
	pos.right = pos.left + 121;
	pos.bottom = pos.top + 36;
	char text[32] = "";

	sprintf(text, "%.0f%%", rainPtc);
	bigFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);

	pos.top += 40;
	pos.bottom = pos.top + 24;

	sprintf(text, "%.0f%% | %.0f%%", onPathWetness, offPathWetness);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
