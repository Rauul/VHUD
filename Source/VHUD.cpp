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


Fuel fuelWidget;
Tyres tyresWidget;
Engine engineWidget;
Rain rainWidget;
Inputs inputsWidget;
FPSMeter fpsWidget;
Menu menu;
Cursor cursor;


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

void VHUD::Load()
{
	fuelWidget.ResetFuelUsage();
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
	inEditMode = false;
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
	tyresWidget.Update(info);
	engineWidget.Update(info);
	inputsWidget.Update(info);
}


void VHUD::InitScreen(const ScreenInfoV01& info)
{
	menu.Init(info);
	cursor.Init(info);
	fuelWidget.Init(info);
	tyresWidget.Init(info);
	engineWidget.Init(info);
	rainWidget.Init(info);
	inputsWidget.Init(info);
	fpsWidget.Init(info);
}

void VHUD::UninitScreen(const ScreenInfoV01& info)
{
	menu.Uninit(info);
	cursor.Uninit(info);
	fuelWidget.Uninit(info);
	tyresWidget.Uninit(info);
	engineWidget.Uninit(info);
	rainWidget.Uninit(info);
	inputsWidget.Uninit(info);
	fpsWidget.Uninit(info);
}

void VHUD::RenderScreenBeforeOverlays(const ScreenInfoV01& info)
{
	if (!inRealtime)
		return;

	MenuEvents();
	UpdatePositions();
	fpsWidget.Update();
	DrawGraphics(info);
}

void VHUD::PreReset(const ScreenInfoV01& info)
{
	menu.PreReset(info);
	cursor.PreReset(info);
	fuelWidget.PreReset(info);
	tyresWidget.PreReset(info);
	engineWidget.PreReset(info);
	rainWidget.PreReset(info);
	inputsWidget.PreReset(info);
	fpsWidget.PreReset(info);
}

void VHUD::PostReset(const ScreenInfoV01& info)
{
	menu.PostReset(info);
	cursor.PostReset(info);
	fuelWidget.PostReset(info);
	tyresWidget.PostReset(info);
	engineWidget.PostReset(info);
	rainWidget.PostReset(info);
	inputsWidget.PostReset(info);
	fpsWidget.PostReset(info);
}

void VHUD::DrawGraphics(const ScreenInfoV01& info)
{
	LPDIRECT3DDEVICE9 d3d = (LPDIRECT3DDEVICE9)info.mDevice;
		
	menu.Draw(inEditMode);
	fuelWidget.Draw(inEditMode);
	tyresWidget.Draw(inEditMode);
	engineWidget.Draw(inEditMode);
	rainWidget.Draw(inEditMode);
	inputsWidget.Draw(inEditMode);
	fpsWidget.Draw(inEditMode);
	cursor.Draw(inEditMode);
}

bool VHUD::MouseIsOver(Menu)
{
	if (cursor.position.y >= menu.position.y && cursor.position.y <= menu.position.y + menu.size.bottom && 
		cursor.position.x >= menu.position.x && cursor.position.x <= menu.position.x + menu.size.right)
		return true;
	else
		return false;
	return false;
}

bool VHUD::MouseIsOver(Fuel)
{
	if (cursor.position.y >= fuelWidget.position.y && cursor.position.y <= fuelWidget.position.y + fuelWidget.size.bottom && 
		cursor.position.x >= fuelWidget.position.x && cursor.position.x <= fuelWidget.position.x + fuelWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Tyres)
{
	if (cursor.position.y >= tyresWidget.position.y && cursor.position.y <= tyresWidget.position.y + tyresWidget.size.bottom && 
		cursor.position.x >= tyresWidget.position.x && cursor.position.x <= tyresWidget.position.x + tyresWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Engine)
{
	if (cursor.position.y >= engineWidget.position.y && cursor.position.y <= engineWidget.position.y + engineWidget.size.bottom &&
		cursor.position.x >= engineWidget.position.x && cursor.position.x <= engineWidget.position.x + engineWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Rain)
{
	if (cursor.position.y >= rainWidget.position.y && cursor.position.y <= rainWidget.position.y + rainWidget.size.bottom &&
		cursor.position.x >= rainWidget.position.x && cursor.position.x <= rainWidget.position.x + rainWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Inputs)
{
	if (cursor.position.y >= inputsWidget.position.y && cursor.position.y <= inputsWidget.position.y + inputsWidget.size.bottom &&
		cursor.position.x >= inputsWidget.position.x && cursor.position.x <= inputsWidget.position.x + inputsWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(FPSMeter)
{
	if (cursor.position.y >= fpsWidget.position.y && cursor.position.y <= fpsWidget.position.y + fpsWidget.size.bottom &&
		cursor.position.x >= fpsWidget.position.x && cursor.position.x <= fpsWidget.position.x + fpsWidget.size.right)
		return true;
	else
		return false;
}

void VHUD::UpdatePositions()
{
	if (!inEditMode)
		return;

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		if (MouseIsOver(fuelWidget) && (cursor.lockedTo == Cursor::LockedTo::Fuel || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Fuel;
			fuelWidget.UpdatePosition();
		}
		else if (MouseIsOver(tyresWidget) && (cursor.lockedTo == Cursor::LockedTo::Tyres || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Tyres;
			tyresWidget.UpdatePosition();
		}
		else if (MouseIsOver(engineWidget) && (cursor.lockedTo == Cursor::LockedTo::Engine || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Engine;
			engineWidget.UpdatePosition();
		}
		else if (MouseIsOver(rainWidget) && (cursor.lockedTo == Cursor::LockedTo::Rain || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Rain;
			rainWidget.UpdatePosition();
		}
		else if (MouseIsOver(inputsWidget) && (cursor.lockedTo == Cursor::LockedTo::Inputs || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Inputs;
			inputsWidget.UpdatePosition();
		}
		else if (MouseIsOver(fpsWidget) && (cursor.lockedTo == Cursor::LockedTo::FPSMeter || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::FPSMeter;
			fpsWidget.UpdatePosition();
		}
	}
	else
	{
		cursor.lockedTo = Cursor::LockedTo::None;
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
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 && cursor.position.x <= menu.position.x + 69 + 68)
		{
			menu.mouseInSlot = 1;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					tyresWidget.enabled = !tyresWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 2 && cursor.position.x <= menu.position.x + 69 + 68 * 2)
		{
			menu.mouseInSlot = 2;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					engineWidget.enabled = !engineWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 3 && cursor.position.x <= menu.position.x + 69 + 68 * 3)
		{
			menu.mouseInSlot = 3;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					rainWidget.enabled = !rainWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 4 && cursor.position.x <= menu.position.x + 69 + 68 * 4)
		{
			menu.mouseInSlot = 4;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					inputsWidget.enabled = !inputsWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 5 && cursor.position.x <= menu.position.x + 69 + 68 * 5)
		{
			menu.mouseInSlot = 5;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					fpsWidget.enabled = !fpsWidget.enabled;
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
	if (!inRealtime)
		return;

	rainWidget.Update(info);
}
