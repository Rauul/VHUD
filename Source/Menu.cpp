#include "Fuel.hpp"
#include "VHUD.hpp"
#include "Menu.hpp"

void Menu::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &fuelIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &tyresIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &engineIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &rainIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &inputsIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &fpsIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &lightIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &gearIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &gridIconSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &brakesIconSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, FUEL_ICON, &fuelIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, TYRES_ICON, &tyresIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, ENGINE_ICON, &engineIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, RAIN_ICON, &rainIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, INPUTS_ICON, &inputsIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, FPS_ICON, &fpsIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, STARTINGLIGHTS_ICON, &lightIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, GEAR_ICON, &gearIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, GRID_ICON, &gridIconTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BRAKES_ICON, &brakesIconTexture);

	position.x = info.mWidth / 2 - size.right / 2;
	position.y = info.mHeight / 2 - size.bottom / 2;
}

void Menu::Uninit(const ScreenInfoV01 & info)
{
	if (boxTexture) {
		boxTexture->Release();
		boxTexture = NULL;
	}
	if (boxSprite) {
		boxSprite->Release();
		boxSprite = NULL;
	}
	if (fuelIconTexture) {
		fuelIconTexture->Release();
		fuelIconTexture = NULL;
	}
	if (fuelIconSprite) {
		fuelIconSprite->Release();
		fuelIconSprite = NULL;
	}
	if (tyresIconTexture) {
		tyresIconTexture->Release();
		tyresIconTexture = NULL;
	}
	if (tyresIconSprite) {
		tyresIconSprite->Release();
		tyresIconSprite = NULL;
	}
	if (engineIconTexture) {
		engineIconTexture->Release();
		engineIconTexture = NULL;
	}
	if (engineIconSprite) {
		engineIconSprite->Release();
		engineIconSprite = NULL;
	}
	if (rainIconTexture) {
		rainIconTexture->Release();
		rainIconTexture = NULL;
	}
	if (rainIconSprite) {
		rainIconSprite->Release();
		rainIconSprite = NULL;
	}
	if (inputsIconTexture) {
		inputsIconTexture->Release();
		inputsIconTexture = NULL;
	}
	if (inputsIconSprite) {
		inputsIconSprite->Release();
		inputsIconSprite = NULL;
	}
	if (fpsIconTexture) {
		fpsIconTexture->Release();
		fpsIconTexture = NULL;
	}
	if (fpsIconSprite) {
		fpsIconSprite->Release();
		fpsIconSprite = NULL;
	}
	if (lightIconTexture) {
		lightIconTexture->Release();
		lightIconTexture = NULL;
	}
	if (lightIconSprite) {
		lightIconSprite->Release();
		lightIconSprite = NULL;
	}
	if (gearIconTexture) {
		gearIconTexture->Release();
		gearIconTexture = NULL;
	}
	if (gearIconSprite) {
		gearIconSprite->Release();
		gearIconSprite = NULL;
	}
	if (gridIconTexture) {
		gridIconTexture->Release();
		gridIconTexture = NULL;
	}
	if (gridIconSprite) {
		gridIconSprite->Release();
		gridIconSprite = NULL;
	}
	if (brakesIconTexture) {
		brakesIconTexture->Release();
		brakesIconTexture = NULL;
	}
	if (brakesIconSprite) {
		brakesIconSprite->Release();
		brakesIconSprite = NULL;
	}
}

void Menu::PreReset(const ScreenInfoV01 & info)
{
	if (fuelIconSprite)
		fuelIconSprite->OnLostDevice();
	if (tyresIconSprite)
		tyresIconSprite->OnLostDevice();
	if (engineIconSprite)
		engineIconSprite->OnLostDevice();
	if (rainIconSprite)
		rainIconSprite->OnLostDevice();
	if (inputsIconSprite)
		inputsIconSprite->OnLostDevice();
	if (fpsIconSprite)
		fpsIconSprite->OnLostDevice();
	if (lightIconSprite)
		lightIconSprite->OnLostDevice();
	if (gearIconSprite)
		gearIconSprite->OnLostDevice();
	if (gridIconSprite)
		gridIconSprite->OnLostDevice();
	if (brakesIconSprite)
		brakesIconSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
}

void Menu::PostReset(const ScreenInfoV01 & info)
{
	if (fuelIconSprite)
		fuelIconSprite->OnResetDevice();
	if (tyresIconSprite)
		tyresIconSprite->OnResetDevice();
	if (engineIconSprite)
		engineIconSprite->OnResetDevice();
	if (rainIconSprite)
		rainIconSprite->OnResetDevice();
	if (inputsIconSprite)
		inputsIconSprite->OnResetDevice();
	if (fpsIconSprite)
		fpsIconSprite->OnResetDevice();
	if (lightIconSprite)
		lightIconSprite->OnResetDevice();
	if (gearIconSprite)
		gearIconSprite->OnResetDevice();
	if (gridIconSprite)
		gridIconSprite->OnResetDevice();
	if (brakesIconSprite)
		brakesIconSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
}

void Menu::Draw(bool inEditMode)
{
	if (!inEditMode)
		return;

	DrawBox();
	DrawIcon(0, fuelIconSprite, fuelIconTexture);
	DrawIcon(1, tyresIconSprite, tyresIconTexture);
	DrawIcon(2, brakesIconSprite, brakesIconTexture);
	DrawIcon(3, engineIconSprite, engineIconTexture);
	DrawIcon(4, rainIconSprite, rainIconTexture);
	DrawIcon(5, gearIconSprite, gearIconTexture);
	DrawIcon(6, inputsIconSprite, inputsIconTexture);
	DrawIcon(7, lightIconSprite, lightIconTexture);
	DrawIcon(8, gridIconSprite, gridIconTexture);
	DrawIcon(9, fpsIconSprite, fpsIconTexture);
}

void Menu::DrawBox()
{
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	boxSprite->Draw(boxTexture, &size, NULL, &position, backgroundColor);
	boxSprite->End();

	if (useBorder)
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

void Menu::DrawIcon(int slot, LPD3DXSPRITE& sprite, LPDIRECT3DTEXTURE9& texture)
{
	RECT size = { 0, 0, 64, 64 };
	D3DXVECTOR3 pos;
	pos.x = position.x + 5 + (slot * 68);
	pos.y = position.y + 5;
	pos.z = 0;

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &size, NULL, &pos, mouseInSlot == slot ? iconMouseOverColor : iconColor);
	sprite->End();

	//
	// Border
	//
	RECT borderSize = size;
	D3DXVECTOR3 borderPosition = pos;

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
	borderPosition.y = pos.y + 1;
	boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
	boxSprite->End();

	// Right
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	borderPosition.x += size.right - 1;
	boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
	boxSprite->End();
}
