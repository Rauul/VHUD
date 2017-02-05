#include "Fuel.hpp"
#include "VHUD.hpp"
#include "Menu.hpp"

Menu::Menu(int x)
{
	position.x = 200;
	position.y = 200;
	position.z = 0;
	size.left = 0;
	size.top = 0;
	size.right = 1337;
	size.bottom = 140;
	backgroundColor = 0xFF000000;
	boarderColor = 0xFFAAAAAA;
	useBoarder = true;
}

void Menu::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &fuelIconSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BIG_FUEL_ICON, &fuelIconTexture);
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
}

void Menu::PreReset(const ScreenInfoV01 & info)
{
	if (fuelIconSprite)
		fuelIconSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
}

void Menu::PostReset(const ScreenInfoV01 & info)
{
	if (fuelIconSprite)
		fuelIconSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
}

void Menu::Draw()
{
	DrawBox();
	DrawIcon(0, fuelIconSprite, fuelIconTexture);
}

void Menu::DrawBox()
{
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	boxSprite->Draw(boxTexture, &size, NULL, &position, backgroundColor);
	boxSprite->End();

	if (useBoarder)
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
	RECT size = { 0, 0, 128, 128 };
	D3DXVECTOR3 pos;
	pos.x = position.x + 6 + (slot * 133);
	pos.y = position.y + 6;
	pos.z = 0;
	D3DCOLOR color = 0xFFFFFFFF;

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &size, NULL, &pos, color);
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
