#include "Gear.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Gear::Init(const ScreenInfoV01 & info)
{
	GetPrivateProfileString("Config", "Font", "Tahoma", smallFontDesc.FaceName, 32, CONFIG_FILE);
	smallFontDesc.Height = GetPrivateProfileInt("Config", "SmallFontSize", 20, CONFIG_FILE);
	GetPrivateProfileString("Config", "Font", "Tahoma", gearFontDesc.FaceName, 32, CONFIG_FILE);
	gearFontDesc.Height = GetPrivateProfileInt("Config", "GearFontSize", 60, CONFIG_FILE);

	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &gearFontDesc, &gearFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);
}

void Gear::Uninit(const ScreenInfoV01 & info)
{
	if (boxTexture) {
		boxTexture->Release();
		boxTexture = NULL;
	}
	if (boxSprite) {
		boxSprite->Release();
		boxSprite = NULL;
	}
	if (gearFont) {
		gearFont->Release();
		gearFont = NULL;
	}
	if (smallFont) {
		smallFont->Release();
		smallFont = NULL;
	}
}

void Gear::PreReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (gearFont)
		gearFont->OnLostDevice(); 
	if (smallFont)
		smallFont->OnLostDevice();
}

void Gear::PostReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (gearFont)
		gearFont->OnResetDevice();
	if (smallFont)
		smallFont->OnResetDevice();
}

void Gear::Update(const TelemInfoV01 & info)
{
	shiftLight = info.mEngineRPM > info.mEngineMaxRPM * 0.97;

	if (info.mGear == -1)
		sprintf(gear, "%c", 'R');

	else if (info.mGear == 0)
		sprintf(gear, "%c", '-');
	else
		sprintf(gear, "%i", info.mGear);

	sprintf(speedKPH, "%.0f", abs(info.mLocalVel.z * 3.6));
}

void Gear::UpdatePosition()
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

void Gear::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawTxt();
}

void Gear::DrawBox(bool inEditMode)
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

void Gear::DrawTxt()
{
	D3DCOLOR color = 0xFFFFFFFF;
	RECT pos;
	pos.left = position.x;
	pos.top = position.y;
	pos.right = pos.left + 64;
	pos.bottom = pos.top + 45;

	if (shiftLight)
		color = 0xFFFF0000;

	gearFont->DrawText(NULL, gear, -1, &pos, DT_CENTER | DT_VCENTER, color);

	color = 0xFFFFFFFF;
	pos.top += 45;
	pos.bottom += 17;

	smallFont->DrawText(NULL, speedKPH, -1, &pos, DT_CENTER | DT_VCENTER, color);
}
