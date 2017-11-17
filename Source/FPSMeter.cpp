#include "FPSMeter.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void FPSMeter::Init(const ScreenInfoV01 & info)
{
	GetPrivateProfileString("Config", "Font", "Tahoma", bigFontDesc.FaceName, 32, CONFIG_FILE);
	bigFontDesc.Height = GetPrivateProfileInt("Config", "BigFontSize", 40, CONFIG_FILE);

	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);
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

long FPSMeter::RoundNum(long num, long multiple)
{
	long rem = num % multiple;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
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
