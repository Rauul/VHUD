#include "Cursor.hpp"
#include "VHUD.hpp"

void Cursor::Init(const ScreenInfoV01 & info)
{
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &cursorSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, MOUSE_TEXTURE, &cursorTexture);
}

void Cursor::Uninit(const ScreenInfoV01 & info)
{
	if (cursorTexture) {
		cursorTexture->Release();
		cursorTexture = NULL;
	}
	if (cursorSprite) {
		cursorSprite->Release();
		cursorSprite = NULL;
	}
}

void Cursor::PreReset(const ScreenInfoV01 & info)
{
	if (cursorSprite)
		cursorSprite->OnLostDevice();
}

void Cursor::PostReset(const ScreenInfoV01 & info)
{
	if (cursorSprite)
		cursorSprite->OnResetDevice();
}

void Cursor::Draw(bool inEditMode)
{
	if (!inEditMode)
		return;
	
	POINT mousePosition;
	if (GetCursorPos(&mousePosition))
	{
		position.x = mousePosition.x;
		position.y = mousePosition.y;
		cursorSprite->Begin(D3DXSPRITE_ALPHABLEND);
		cursorSprite->Draw(cursorTexture, &size, NULL, &position, color);
		cursorSprite->End();
	}
}
