#include "FPSMeter.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void FPSMeter::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);

	position.x = info.mWidth / 2 - size.right / 2;
}

void FPSMeter::Uninit(const ScreenInfoV01 & info)
{
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

void FPSMeter::PreReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (bigFont)
		bigFont->OnLostDevice();
	if (smallFont)
		smallFont->OnLostDevice();
}

void FPSMeter::PostReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (bigFont)
		bigFont->OnResetDevice();
	if (smallFont)
		smallFont->OnResetDevice();
}

void FPSMeter::Update()
{
	frames++;
	if (startTime + 999 < GetTickCount())
	{
		int ms = GetTickCount() - startTime;
		fps = frames * 1000 / ms;
		frames = 0;
		startTime = GetTickCount();
	}
}

void FPSMeter::UpdatePosition()
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

void FPSMeter::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawTxt();
}

void FPSMeter::DrawBox(bool inEditMode)
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

void FPSMeter::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x;
	pos.top = position.y;
	pos.right = pos.left + 64;
	pos.bottom = pos.top + 65;
	char text[32] = "";

	sprintf(text, "%i", fps);
	bigFont->DrawText(NULL, text, -1, &pos, DT_CENTER | DT_VCENTER, color);
}
