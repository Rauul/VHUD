//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Source File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                 PROPRIETARY AND CONFIDENTIAL                            ﬁ
//› Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

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


Fuel fuelWidget(200, 200, 200, 200, 0xFF000000, 0xFFAAAAAA, true);


struct Player
{
	double fuelQuantity;
	double fuelQuantityLastLap;
	double fuelUsedPerLap[5];

	double lastLapStartET;
	double lapTimes[5];
}player;


// VHUD class

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
}


void VHUD::InitScreen(const ScreenInfoV01& info)
{
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, TEXTURE_BACKGROUND, &texture);
	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &sprite);
}

void VHUD::UninitScreen(const ScreenInfoV01& info)
{
	if (texture) {
		texture->Release();
		texture = NULL;
	}
	if (sprite) {
		sprite->Release();
		sprite = NULL;
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
	if (sprite)
		sprite->OnLostDevice();
}

void VHUD::PostReset(const ScreenInfoV01& info)
{
	if (sprite)
		sprite->OnResetDevice();
}

void VHUD::DrawGraphics(const ScreenInfoV01& info)
{
	LPDIRECT3DDEVICE9 d3d = (LPDIRECT3DDEVICE9)info.mDevice;

	DrawBox(fuelWidget.position.x, fuelWidget.position.y, fuelWidget.size.right, fuelWidget.size.bottom, fuelWidget.backgroundColor, fuelWidget.boarderColor, fuelWidget.useBoarder);
}

void VHUD::DrawBox(float posX, float posY, int width, int height, D3DCOLOR backgroundColor, D3DCOLOR boarderColor, bool useBoarder)
{
	RECT size = { 0, 0, width, height };
	D3DXVECTOR3 position = { posX, posY, 0 };
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &size, NULL, &position, backgroundColor);
	sprite->End();

	if (useBoarder)
	{
		RECT borderSize = size;
		D3DXVECTOR3 borderPosition = position;

		// Top
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.bottom = 1;
		sprite->Draw(texture, &borderSize, NULL, &borderPosition, boarderColor);
		sprite->End();

		// Bottom
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.y += height - 1;
		sprite->Draw(texture, &borderSize, NULL, &borderPosition, boarderColor);
		sprite->End();

		// Left
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.right = 1;
		borderSize.bottom = height - 2;
		borderPosition.y = posY + 1;
		sprite->Draw(texture, &borderSize, NULL, &borderPosition, boarderColor);
		sprite->End();

		// Right
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.x += width - 1;
		sprite->Draw(texture, &borderSize, NULL, &borderPosition, boarderColor);
		sprite->End();
	}
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
