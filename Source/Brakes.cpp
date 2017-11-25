#include "Brakes.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Brakes::Init(const ScreenInfoV01 & info)
{
	GetPrivateProfileString("Config", "Font", "Tahoma", smallFontDesc.FaceName, 32, CONFIG_FILE);
	smallFontDesc.Height = GetPrivateProfileInt("Config", "SmallFontSize", 20, CONFIG_FILE);

	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &iconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BRAKES_ICON, &iconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);
}

void Brakes::Uninit(const ScreenInfoV01 & info)
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
	if (smallFont) {
		smallFont->Release();
		smallFont = NULL;
	}
}

void Brakes::PreReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (smallFont)
		smallFont->OnLostDevice();
}

void Brakes::PostReset(const ScreenInfoV01 & info)
{
	if (iconSprite)
		iconSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (smallFont)
		smallFont->OnResetDevice();
}

void Brakes::Update(const TelemInfoV01 & info)
{
	tempFL = info.mWheel[0].mBrakeTemp - 273.15;
	tempFR = info.mWheel[1].mBrakeTemp - 273.15;
	tempRL = info.mWheel[2].mBrakeTemp - 273.15;
	tempRR = info.mWheel[3].mBrakeTemp - 273.15;
}

void Brakes::UpdatePosition()
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

long Brakes::RoundNum(long num, long multiple)
{
	long rem = num % multiple;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
}

void Brakes::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawIcon();
	DrawTxt();
}

void Brakes::DrawBox(bool inEditMode)
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


void Brakes::getHeatMapColor(float *red, float *green, float *blue)
{
	double temperatures[4] = { tempFL, tempFR, tempRL, tempRR };
	double temp = 0;

	for (int i = 0; i < 4; i++)
	{
		if (temperatures[i] > temp)
			temp = temperatures[i];
	}

	float value = (temp / heatMapMax - heatMapMin / (heatMapMax - heatMapMin)); // First we should normalize to a number between 0 and 1
	const int NUM_COLORS = 5;
	static float color[NUM_COLORS][4] = { { 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } }; // A static array of 5 colors using {r,g,b} for each.

	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

	if (value <= 0) { idx1 = idx2 = 0; }    // accounts for an input <=0
	else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }    // accounts for an input >=1
	else
	{
		value = value * (NUM_COLORS - 1);       // Will multiply value by number of colors used - 1..
		idx1 = floor(value);					// Our desired color will be after this index.
		idx2 = idx1 + 1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);		// Distance between the two indexes (0-1).
	}

	*red = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
	*green = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
	*blue = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];
}

void Brakes::DrawIcon()
{
	RECT size = { 0, 0, 64, 64 };
	D3DCOLOR color = 0xFFFFFFFF;

	if (useHeatMap)
	{
		float r,g,b;
		getHeatMapColor(&r, &g, &b);
		color = D3DCOLOR_COLORVALUE(r, g, b, 1);
	}

	iconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	iconSprite->Draw(iconTexture, &size, NULL, &position, color);
	iconSprite->End();
}

void Brakes::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x + 50;
	pos.top = position.y + 10;
	pos.right = pos.left + 120;
	pos.bottom = pos.top + 36;
	char text[32] = "";

	sprintf(text, "%.0f C | %.0f C", tempFL, tempFR);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
	pos.top += 25;
	pos.bottom = pos.top + 24;
	sprintf(text, "%.0f C | %.0f C", tempRL, tempRR);
	smallFont->DrawText(NULL, text, -1, &pos, DT_CENTER, color);
}
