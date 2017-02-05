//���������������������������������������������������������������������������
//�                                                                         �
//� Module: Internals Example Source File                                   �
//�                                                                         �
//� Description: Declarations for the Internals Example Plugin              �
//�                                                                         �
//�                                                                         �
//� This source code module, and all information, data, and algorithms      �
//� associated with it, are part of CUBE technology (tm).                   �
//�                 PROPRIETARY AND CONFIDENTIAL                            �
//� Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. �
//�                                                                         �
//�                                                                         �
//� Change history:                                                         �
//�   tag.2005.11.30: created                                               �
//�                                                                         �
//���������������������������������������������������������������������������

#include "VHUD.hpp"          // corresponding header file
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
#include <vector>

using namespace std;

// plugin information

extern "C" __declspec( dllexport )
const char * __cdecl GetPluginName()                   { return( "ExamplePlugin - 2008.02.13" ); }

extern "C" __declspec( dllexport )
PluginObjectType __cdecl GetPluginType()               { return( PO_INTERNALS ); }

extern "C" __declspec( dllexport )
int __cdecl GetPluginVersion()                         { return( 6 ); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec( dllexport )
PluginObject * __cdecl CreatePluginObject()            { return( (PluginObject *) new VHUD ); }

extern "C" __declspec( dllexport )
void __cdecl DestroyPluginObject( PluginObject *obj )  { delete( (VHUD *) obj ); }


Fuel fuelWidget(200, 200, 218, 65, 0xFF000000, 0xFFAAAAAA, true);


struct Player
{
	double fuelQuantity;
	double fuelQuantityLastLap;
	double fuelUsedPerLap[5];

	double lastLapStartET;
	double lapTimes[5];
}player;




//
// VHUD class
//

void VHUD::Startup(long version)
{
}


void VHUD::Shutdown()
{
}


void VHUD::StartSession()
{
}


void VHUD::EndSession()
{
}


void VHUD::EnterRealtime()
{
	inRealtime = true;
}


void VHUD::ExitRealtime()
{
	inRealtime = false;
}


void VHUD::UpdateTelemetry( const TelemInfoV01& info )
{
	fuelWidget.Update(info);
}


void VHUD::InitScreen(const ScreenInfoV01& info)
{
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, TEXTURE_BACKGROUND, &boxTexture);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, FUEL_ICON, &fuelIconTexture);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &fuelIconSprite);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &BigFontDesc, &big_Font);
}

void VHUD::UninitScreen(const ScreenInfoV01& info)
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
	if (big_Font) {
		big_Font->Release();
		big_Font = NULL;
	}
}

void VHUD::RenderScreenBeforeOverlays(const ScreenInfoV01& info)
{
	if (!inRealtime)
		return;

	DrawGraphics(info);
}

void VHUD::PreReset(const ScreenInfoV01& info)
{
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (fuelIconSprite)
		fuelIconSprite->OnLostDevice();
	if (big_Font)
		big_Font->OnLostDevice();
}

void VHUD::PostReset(const ScreenInfoV01& info)
{
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (fuelIconSprite)
		fuelIconSprite->OnResetDevice();
	if (big_Font)
		big_Font->OnResetDevice();
}

void VHUD::DrawGraphics(const ScreenInfoV01& info)
{
	LPDIRECT3DDEVICE9 d3d = (LPDIRECT3DDEVICE9)info.mDevice;

	DrawBox(fuelWidget.position.x, fuelWidget.position.y, fuelWidget.size.right, fuelWidget.size.bottom, fuelWidget.backgroundColor, fuelWidget.boarderColor, fuelWidget.useBoarder);
	DrawIcon(fuelWidget.position.x, fuelWidget.position.y, 64, 64, 0xFFFFFFFF);
	DrawDText(fuelWidget.quantity, fuelWidget.position.x, fuelWidget.position.y, fuelWidget.size.right, fuelWidget.size.bottom, DT_CENTER, 0xFFFFFFFF);
}

void VHUD::DrawBox(float posX, float posY, int width, int height, D3DCOLOR backgroundColor, D3DCOLOR boarderColor, bool useBoarder)
{
	RECT size = { 0, 0, width, height };
	D3DXVECTOR3 position = { posX, posY, 0 };
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
		borderPosition.y += height - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();

		// Left
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.right = 1;
		borderSize.bottom = height - 2;
		borderPosition.y = posY + 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();

		// Right
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.x += width - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, boarderColor);
		boxSprite->End();
	}
}

void VHUD::DrawIcon(float posX, float posY, int width, int height, D3DCOLOR color)
{
	RECT size = { 0, 0, width, height };
	D3DXVECTOR3 position = { posX, posY, 0 };

	fuelIconSprite->Begin(D3DXSPRITE_ALPHABLEND);
	fuelIconSprite->Draw(fuelIconTexture, &size, NULL, &position, color);
	fuelIconSprite->End();
}

void VHUD::DrawDText(double value, float posX, float posY, int width, int height, int align, D3DCOLOR color)
{
	RECT position;
	position.left = posX;
	position.top = posY;
	position.right = posX + width;
	position.bottom = posY + height;

	char text[64] = "";

	sprintf(text, "%.2f", value);
	big_Font->DrawText(NULL, text, -1, &position, align, color);
}

void VHUD::UpdateFuel(const TelemInfoV01& tinfo, const ScoringInfoV01& sinfo)
{
	player.fuelQuantity = tinfo.mFuel;

	if (NewLapStarted(tinfo, sinfo))
	{
		
	}
}

bool VHUD::NewLapStarted(const TelemInfoV01& tinfo, const ScoringInfoV01& sinfo)
{
	if (tinfo.mLapStartET > player.lastLapStartET)
		return true;

	return false;
}


void VHUD::UpdateScoring( const ScoringInfoV01& info )
{
}
