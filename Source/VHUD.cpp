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


Fuel fuelWidget(1);
Menu menu(1);
Cursor cursor;


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
	inEditMode = false;
	fuelWidget.firstUpdate = true;
}


void VHUD::ExitRealtime()
{
	inRealtime = false;
}


void VHUD::UpdateTelemetry( const TelemInfoV01& info )
{
	if (!inRealtime)
		return;

	if (KEY_DOWN(DEFAULT_EDIT_KEY))
	{
		if (!editkeyDownLastFrame)
		{
			inEditMode = !inEditMode;
			/*if (!inEditMode)
				SaveConfig(grid, CONFIG_FILE);*/
		}
		editkeyDownLastFrame = true;
	}
	else
	{
		editkeyDownLastFrame = false;
	}

	fuelWidget.Update(info);
}


void VHUD::InitScreen(const ScreenInfoV01& info)
{
	fuelWidget.Init(info);
	menu.Init(info);
	cursor.Init(info);
}

void VHUD::UninitScreen(const ScreenInfoV01& info)
{
	fuelWidget.Uninit(info);
	menu.Uninit(info);
	cursor.Uninit(info);
}

void VHUD::RenderScreenBeforeOverlays(const ScreenInfoV01& info)
{
	if (!inRealtime)
		return;

	MenuEvents();
	UpdatePositions();
	DrawGraphics(info);
}

void VHUD::PreReset(const ScreenInfoV01& info)
{
	fuelWidget.PreReset(info);
	menu.PreReset(info);
	cursor.PreReset(info);
}

void VHUD::PostReset(const ScreenInfoV01& info)
{
	fuelWidget.PostReset(info);
	menu.PostReset(info);
	cursor.PostReset(info);
}

void VHUD::DrawGraphics(const ScreenInfoV01& info)
{
	LPDIRECT3DDEVICE9 d3d = (LPDIRECT3DDEVICE9)info.mDevice;

	
	menu.Draw(inEditMode);
	fuelWidget.Draw(inEditMode);
	cursor.Draw(inEditMode);
}

bool VHUD::NewLapStarted(const TelemInfoV01& info)
{
	if (info.mLapStartET > player.lastLapStartET)
		return true;

	return false;
}

bool VHUD::MouseIsOver(Fuel)
{
	if (cursor.position.y >= fuelWidget.position.y && cursor.position.y <= fuelWidget.position.y + fuelWidget.size.bottom && cursor.position.x >= fuelWidget.position.x && cursor.position.x <= fuelWidget.position.x + fuelWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Menu)
{
	if (cursor.position.y >= menu.position.y && cursor.position.y <= menu.position.y + menu.size.bottom && cursor.position.x >= menu.position.x && cursor.position.x <= menu.position.x + menu.size.right)
		return true;
	else
		return false;
	return false;
}

void VHUD::UpdatePositions()
{
	if (!inEditMode || (GetKeyState(VK_LBUTTON) & 0x100) == 0)
		return;

	if (MouseIsOver(fuelWidget) && (cursor.lockedTo == Cursor::LockedTo::Fuel || cursor.lockedTo == Cursor::LockedTo::None))
	{
		cursor.lockedTo == Cursor::LockedTo::Fuel;
		fuelWidget.UpdatePosition();
	}
}

void VHUD::MenuEvents()
{
	if (!inEditMode)
		return;

	menu.mouseInSlot = -1;

	if (MouseIsOver(menu) && cursor.lockedTo == Cursor::LockedTo::None)
	{
		if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 && cursor.position.x <= menu.position.x + 69)
		{
			menu.mouseInSlot = 0;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					fuelWidget.enabled = !fuelWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
	}
}

void VHUD::UpdateScoring( const ScoringInfoV01& info )
{
}
