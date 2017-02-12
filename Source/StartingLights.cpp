#include "StartingLights.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void StartingLights::ResetLights()
{
	numRedLights = numLights = 0;
}

void StartingLights::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &offLightSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &redLightSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &yellowLightSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &greenLightSprite);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, STARTINGLIGHT_OFF, &offLightTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, STARTINGLIGHT_RED, &redLightTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, STARTINGLIGHT_YELLOW, &yellowLightTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, STARTINGLIGHT_GREEN, &greenLightTexture);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);

	position.x = info.mWidth / 2 - size.right / 2;
	position.y = info.mHeight * 0.2;
}

void StartingLights::Uninit(const ScreenInfoV01 & info)
{
	if (offLightTexture) {
		offLightTexture->Release();
		offLightTexture = NULL;
	}
	if (offLightSprite) {
		offLightSprite->Release();
		offLightSprite = NULL;
	}
	if (redLightTexture) {
		redLightTexture->Release();
		redLightTexture = NULL;
	}
	if (redLightSprite) {
		redLightSprite->Release();
		redLightSprite = NULL;
	}
	if (yellowLightTexture) {
		yellowLightTexture->Release();
		yellowLightTexture = NULL;
	}
	if (yellowLightSprite) {
		yellowLightSprite->Release();
		yellowLightSprite = NULL;
	}
	if (greenLightTexture) {
		greenLightTexture->Release();
		greenLightTexture = NULL;
	}
	if (greenLightSprite) {
		greenLightSprite->Release();
		greenLightSprite = NULL;
	}
	if (boxTexture) {
		boxTexture->Release();
		boxTexture = NULL;
	}
	if (boxSprite) {
		boxSprite->Release();
		boxSprite = NULL;
	}
}

void StartingLights::PreReset(const ScreenInfoV01 & info)
{
	if (offLightSprite)
		offLightSprite->OnLostDevice();
	if (redLightSprite)
		redLightSprite->OnLostDevice();
	if (yellowLightSprite)
		yellowLightSprite->OnLostDevice();
	if (greenLightSprite)
		greenLightSprite->OnLostDevice();
	if (boxSprite)
		boxSprite->OnLostDevice();
}

void StartingLights::PostReset(const ScreenInfoV01 & info)
{
	if (offLightSprite)
		offLightSprite->OnResetDevice();
	if (redLightSprite)
		redLightSprite->OnResetDevice();
	if (yellowLightSprite)
		yellowLightSprite->OnResetDevice();
	if (greenLightSprite)
		greenLightSprite->OnResetDevice();
	if (boxSprite)
		boxSprite->OnResetDevice();
}

void StartingLights::Update(const ScoringInfoV01 & info)
{
	numLights = info.mStartLight;
	numRedLights = info.mNumRedLights;
	
	if (playerSlot < 0 || info.mVehicle[playerSlot].mControl != 0)
	{
		for (long i = 0; i < info.mNumVehicles; i++)
		{
			if (info.mVehicle[i].mControl == 0)
			{
				playerSlot = i;
				break;
			}
		}
	}

	inPits = info.mVehicle[playerSlot].mInPits;

	if (info.mGamePhase == 4 || inPits || ( info.mGamePhase == 5 && info.mSession > 9 && numLights > numRedLights))
		showLights = true;
	else
		showLights = false;
}

void StartingLights::Update(const TelemInfoV01 & info)
{
	return;
}

void StartingLights::UpdatePosition()
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

void StartingLights::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	if (showLights || inEditMode)
		DrawLights(inEditMode);
}

void StartingLights::DrawLights(bool inEditMode)
{
	RECT rect = { 0, 0, 128, 128 };
	D3DCOLOR color = 0xDDFFFFFF;
	D3DXVECTOR3 pos = position;

	pos.x += size.right / 2;
	pos.x -= (42 * numRedLights) + 24;
	pos.y -= size.bottom / 2 - 24;

	for (int i = 0; i < numRedLights; i++)
	{
		if (inPits || inEditMode)
		{
			yellowLightSprite->Begin(D3DXSPRITE_ALPHABLEND);
			yellowLightSprite->Draw(yellowLightTexture, &rect, NULL, &pos, color);
			yellowLightSprite->End();
		}
		else if (numLights > numRedLights)
		{
			greenLightSprite->Begin(D3DXSPRITE_ALPHABLEND);
			greenLightSprite->Draw(greenLightTexture, &rect, NULL, &pos, color);
			greenLightSprite->End();
		}
		else if (numLights > i)
		{
			redLightSprite->Begin(D3DXSPRITE_ALPHABLEND);
			redLightSprite->Draw(redLightTexture, &rect, NULL, &pos, color);
			redLightSprite->End();
		}
		else
		{
			offLightSprite->Begin(D3DXSPRITE_ALPHABLEND);
			offLightSprite->Draw(offLightTexture, &rect, NULL, &pos, color);
			offLightSprite->End();
		}
		pos.x += 84;
	}
}
