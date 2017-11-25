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

extern "C" __declspec(dllexport)
const char * __cdecl GetPluginName() { return("ExamplePlugin - 2008.02.13"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(6); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject *) new VHUD); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject *obj) { delete((VHUD *)obj); }


Fuel fuelWidget;
Tyres tyresWidget;
Engine engineWidget;
Rain rainWidget;
Inputs inputsWidget;
FPSMeter fpsWidget;
StartingLights lightsWidget;
Gear gearWidget;
Grid gridWidget;
Brakes brakesWidget;
Menu menu;
Cursor cursor;

static inline LPCSTR NextToken(LPCSTR pArg)
{
	// find next null with strchr and
	// point to the char beyond that.
	return strchr(pArg, '\0') + 1;
}

//
// VHUD class
//

void VHUD::Startup(long version)
{
}


void VHUD::Shutdown()
{
}


void VHUD::StartSession(const ScoringInfoV01& info)
{
	lightsWidget.ResetLights();
}


void VHUD::EndSession()
{
}

void VHUD::Load()
{
	timeLoaded = GetTickCount();
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
	SaveConfig(CONFIG_FILE);
}


void VHUD::UpdateTelemetry(const TelemInfoV01& info)
{
	if (!inRealtime || !isPlayer)
		return;
	
	if (KEY_DOWN(editKey))
	{
		if (!editkeyDownLastFrame)
		{
			inEditMode = !inEditMode;
			if (!inEditMode)
				SaveConfig(CONFIG_FILE);
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
	gearWidget.Update(info);
	brakesWidget.Update(info);
}

void VHUD::UpdateScoring(const ScoringInfoV01& info)
{
	inRealtime = info.mInRealtime;
	isPlayer = IsPlayer(info);

	if (!inRealtime || !isPlayer)
		return;
	
	fuelWidget.Update(info);
	rainWidget.Update(info);
	lightsWidget.Update(info);
	gridWidget.Update(info);
}

void VHUD::InitScreen(const ScreenInfoV01& info)
{
	screenCenter = info.mWidth / 2;
	screenHeight = info.mHeight;

	gScaleFactor = (float)info.mHeight / 1080.0f;
	
	//D3DXMatrixTranslation(&mat, (1.0f / gScaleFactor) * 0.5f, 1.0f / gScaleFactor * 0.5f, 0.0f);

	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &splashSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, SPLASH_TEXTURE, &splashTexture);

	/*D3DXCreateTextureFromFileEx((LPDIRECT3DDEVICE9)info.mDevice, SPLASH_TEXTURE, 512, 256, D3DX_DEFAULT, 
		0, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &splashTexture);*/

	LoadConfig(CONFIG_FILE);

	menu.Init(info);
	cursor.Init(info);
	fuelWidget.Init(info);
	tyresWidget.Init(info);
	engineWidget.Init(info);
	rainWidget.Init(info);
	inputsWidget.Init(info);
	fpsWidget.Init(info);
	lightsWidget.Init(info);
	gearWidget.Init(info, gScaleFactor);
	gridWidget.Init(info);
	brakesWidget.Init(info);
}

void VHUD::UninitScreen(const ScreenInfoV01& info)
{
	if (splashTexture) {
		splashTexture->Release();
		splashTexture = NULL;
	}
	if (splashSprite) {
		splashSprite->Release();
		splashSprite = NULL;
	}

	menu.Uninit(info);
	cursor.Uninit(info);
	fuelWidget.Uninit(info);
	tyresWidget.Uninit(info);
	engineWidget.Uninit(info);
	rainWidget.Uninit(info);
	inputsWidget.Uninit(info);
	fpsWidget.Uninit(info);
	lightsWidget.Uninit(info);
	gearWidget.Uninit(info);
	gridWidget.Uninit(info);
	brakesWidget.Uninit(info);
}

void VHUD::RenderScreenBeforeOverlays(const ScreenInfoV01& info)
{
		//DrawSplashScreen();

	if (!inRealtime || !isPlayer)
		return;

	if (inEditMode && KEY_DOWN(resetKey))
		ResetPositions(info);

	MenuEvents();
	UpdatePositions();
	fpsWidget.Update();
	DrawGraphics(info);
}

void VHUD::PreReset(const ScreenInfoV01& info)
{
	if (splashSprite)
		splashSprite->OnLostDevice();

	menu.PreReset(info);
	cursor.PreReset(info);
	fuelWidget.PreReset(info);
	tyresWidget.PreReset(info);
	engineWidget.PreReset(info);
	rainWidget.PreReset(info);
	inputsWidget.PreReset(info);
	fpsWidget.PreReset(info);
	lightsWidget.PreReset(info);
	gearWidget.PreReset(info);
	gridWidget.PreReset(info);
	brakesWidget.PreReset(info);
}

void VHUD::PostReset(const ScreenInfoV01& info)
{
	if (splashSprite)
		splashSprite->OnResetDevice();

	menu.PostReset(info);
	cursor.PostReset(info);
	fuelWidget.PostReset(info);
	tyresWidget.PostReset(info);
	engineWidget.PostReset(info);
	rainWidget.PostReset(info);
	inputsWidget.PostReset(info);
	fpsWidget.PostReset(info);
	lightsWidget.PostReset(info);
	gearWidget.PostReset(info);
	gridWidget.PostReset(info);
	brakesWidget.PostReset(info);
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
	lightsWidget.Draw(inEditMode);
	gearWidget.Draw(inEditMode);
	gridWidget.Draw(inEditMode);
	brakesWidget.Draw(inEditMode);
	cursor.Draw(inEditMode);
}

bool VHUD::WantsToDisplayMessage(MessageInfoV01 & msgInfo)
{
	if (!inRealtime || displayedWelcomeMessage)
		return false;

	msgInfo.mDestination = 0;
	msgInfo.mTranslate = 0;
	sprintf(msgInfo.mText, "V-HUD " PLUGIN_VERSION);
	displayedWelcomeMessage = true;
	return true;
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

bool VHUD::MouseIsOver(StartingLights)
{
	if (cursor.position.y >= lightsWidget.position.y && cursor.position.y <= lightsWidget.position.y + lightsWidget.size.bottom &&
		cursor.position.x >= lightsWidget.position.x && cursor.position.x <= lightsWidget.position.x + lightsWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Gear)
{
	if (cursor.position.y >= gearWidget.position.y && cursor.position.y <= gearWidget.position.y + gearWidget.size.bottom &&
		cursor.position.x >= gearWidget.position.x && cursor.position.x <= gearWidget.position.x + gearWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Grid)
{
	if (cursor.position.y >= gridWidget.position.y && cursor.position.y <= gridWidget.position.y + gridWidget.size.bottom &&
		cursor.position.x >= gridWidget.position.x && cursor.position.x <= gridWidget.position.x + gridWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::MouseIsOver(Brakes)
{
	if (cursor.position.y >= brakesWidget.position.y && cursor.position.y <= brakesWidget.position.y + brakesWidget.size.bottom &&
		cursor.position.x >= brakesWidget.position.x && cursor.position.x <= brakesWidget.position.x + brakesWidget.size.right)
		return true;
	else
		return false;
}

bool VHUD::IsPlayer(const ScoringInfoV01 & info)
{
	if (info.mNumVehicles < 1)
		return false;

	if (info.mVehicle[playerSlot].mIsPlayer)
		return true;

	for (long i = 0; i < info.mNumVehicles; ++i)
	{
		if (info.mVehicle[i].mIsPlayer)
		{
			playerSlot = i;
			return true;
		}
	}
	return false;
}

void VHUD::UpdatePositions()
{
	if (!inEditMode)
		return;

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		if (fuelWidget.enabled && MouseIsOver(fuelWidget) && (cursor.lockedTo == Cursor::LockedTo::Fuel || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Fuel;
			fuelWidget.UpdatePosition();
		}
		else if (tyresWidget.enabled && MouseIsOver(tyresWidget) && (cursor.lockedTo == Cursor::LockedTo::Tyres || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Tyres;
			tyresWidget.UpdatePosition();
		}
		else if (engineWidget.enabled && MouseIsOver(engineWidget) && (cursor.lockedTo == Cursor::LockedTo::Engine || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Engine;
			engineWidget.UpdatePosition();
		}
		else if (rainWidget.enabled && MouseIsOver(rainWidget) && (cursor.lockedTo == Cursor::LockedTo::Rain || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Rain;
			rainWidget.UpdatePosition();
		}
		else if (inputsWidget.enabled && MouseIsOver(inputsWidget) && (cursor.lockedTo == Cursor::LockedTo::Inputs || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Inputs;
			inputsWidget.UpdatePosition();
		}
		else if (fpsWidget.enabled && MouseIsOver(fpsWidget) && (cursor.lockedTo == Cursor::LockedTo::FPSMeter || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::FPSMeter;
			fpsWidget.UpdatePosition();
		}
		else if (lightsWidget.enabled && MouseIsOver(lightsWidget) && (cursor.lockedTo == Cursor::LockedTo::StartingLights || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::StartingLights;
			lightsWidget.UpdatePosition();
		}
		else if (gearWidget.enabled && MouseIsOver(gearWidget) && (cursor.lockedTo == Cursor::LockedTo::Gear || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Gear;
			gearWidget.UpdatePosition();
		}
		else if (gridWidget.enabled && MouseIsOver(gridWidget) && (cursor.lockedTo == Cursor::LockedTo::Grid || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Grid;
			gridWidget.UpdatePosition();
		}
		else if (brakesWidget.enabled && MouseIsOver(brakesWidget) && (cursor.lockedTo == Cursor::LockedTo::Brakes || cursor.lockedTo == Cursor::LockedTo::None))
		{
			cursor.lockedTo = Cursor::LockedTo::Brakes;
			brakesWidget.UpdatePosition();
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
					brakesWidget.enabled = !brakesWidget.enabled;
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
			cursor.position.x >= menu.position.x + 5 + 68 * 4 && cursor.position.x <= menu.position.x + 69 + 68 * 4)
		{
			menu.mouseInSlot = 4;

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
			cursor.position.x >= menu.position.x + 5 + 68 * 5 && cursor.position.x <= menu.position.x + 69 + 68 * 5)
		{
			menu.mouseInSlot = 5;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					gearWidget.enabled = !gearWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 6 && cursor.position.x <= menu.position.x + 69 + 68 * 6)
		{
			menu.mouseInSlot = 6;

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
			cursor.position.x >= menu.position.x + 5 + 68 * 7 && cursor.position.x <= menu.position.x + 69 + 68 * 7)
		{
			menu.mouseInSlot = 7;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					lightsWidget.enabled = !lightsWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 8 && cursor.position.x <= menu.position.x + 69 + 68 * 8)
		{
			menu.mouseInSlot = 8;

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				if (!mouseDownLastFrame)
				{
					gridWidget.enabled = !gridWidget.enabled;
				}
				mouseDownLastFrame = true;
			}
			else
			{
				mouseDownLastFrame = false;
			}
		}
		else if (cursor.position.y >= menu.position.y + 5 && cursor.position.y <= menu.position.y + 69 &&
			cursor.position.x >= menu.position.x + 5 + 68 * 9 && cursor.position.x <= menu.position.x + 69 + 68 * 9)
		{
			menu.mouseInSlot = 9;

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

LPCSTR ConvertFloatToLPCSTR(float input)
{
	LPCSTR output;
	std::stringstream s;
	s << input;
	std::string ws = s.str();
	output = ws.c_str();
	return output;
}

void VHUD::ResetPositions(const ScreenInfoV01& info)
{
	float y = info.mHeight * 0.15;
	lightsWidget.position = { screenCenter, y, 0 };
	//y += 128;
	fuelWidget.position = { screenCenter, y, 0 };
	//y += 65;
	tyresWidget.position = { screenCenter, y, 0 };
	//y += 65;
	engineWidget.position = { screenCenter, y, 0 };
	//y += 65;
	rainWidget.position = { screenCenter, y, 0 };
	//y += 65;
	inputsWidget.position = { screenCenter, y, 0 };
	//y += 65;
	fpsWidget.position = { screenCenter, y, 0 };
	//y += 65;
	gearWidget.position = { screenCenter, y, 0 };
	//y += 65;
	gridWidget.position = { screenCenter, y, 0 };
	//y += 65;
	brakesWidget.position = { screenCenter, y, 0 };
}

void VHUD::DrawSplashScreen()
{
	if (true)	//(timeLoaded + 30000 > GetTickCount())
	{
		RECT size = { 0, 0, 512, 256 };
		D3DXVECTOR3 position = { 0, 0, 0 };
		position.x = screenCenter - 256 * gScaleFactor;
		position.y = screenHeight / 2 - 128 * gScaleFactor;

		D3DXVECTOR3 spriteCenter = { screenCenter - 256 * gScaleFactor, screenHeight / 2 - 128 * gScaleFactor,0 };
		D3DXMATRIX mat;
		D3DXVECTOR3 scaling(gScaleFactor, gScaleFactor, 0);
		D3DXMatrixTransformation(&mat, &spriteCenter, NULL, &scaling, NULL, NULL, NULL);
		splashSprite->SetTransform(&mat);

		splashSprite->Begin(D3DXSPRITE_ALPHABLEND);
		splashSprite->Draw(splashTexture, &size, NULL, &position, 0x88FFFFFF);
		splashSprite->End();
	}
}

void VHUD::LoadConfig(const char * ini_file)
{
	// [Config]
	fuelWidget.useBorder = tyresWidget.useBorder = engineWidget.useBorder = rainWidget.useBorder = inputsWidget.useBorder = 
		fpsWidget.useBorder = gearWidget.useBorder = gridWidget.useBorder = brakesWidget.useBorder = 
		GetPrivateProfileInt("Config", "Borders", USE_BORDERS, ini_file);
	fuelWidget.backgroundColor = tyresWidget.backgroundColor = engineWidget.backgroundColor = rainWidget.backgroundColor = inputsWidget.backgroundColor = fpsWidget.backgroundColor = 
		gearWidget.backgroundColor = gridWidget.backgroundColor = brakesWidget.backgroundColor =
		GetPrivateProfileInt("Config", "BackgroundColor", BACKGROUND_COLOR, ini_file);
	fuelWidget.borderColor = tyresWidget.borderColor = engineWidget.borderColor = rainWidget.borderColor = inputsWidget.borderColor = fpsWidget.borderColor = gearWidget.borderColor = 
		gridWidget.borderColor = brakesWidget.borderColor =
		GetPrivateProfileInt("Config", "BorderColor", BORDER_COLOR, ini_file);
	gScaleFactor = (float)GetPrivateProfileInt("Config", "ScalePct", gScaleFactor * 100, ini_file) / 100;


	// [Controls]
	editKey = GetPrivateProfileInt("Controls", "EditKey", DEFAULT_EDIT_KEY, ini_file);
	resetKey = GetPrivateProfileInt("Controls", "ResetKey", DEFAULT_RESET_KEY, ini_file);


	// [Fuel]
	fuelWidget.enabled = GetPrivateProfileInt("Fuel", "Enabled", true, ini_file);
	fuelWidget.position.x = (int)GetPrivateProfileInt("Fuel", "PosX", 0, ini_file) + screenCenter;
	fuelWidget.position.y = GetPrivateProfileInt("Fuel", "PosY", fuelWidget.position.y, ini_file);


	// [Tyres]
	tyresWidget.enabled = GetPrivateProfileInt("Tyres", "Enabled", true, ini_file);
	tyresWidget.position.x = (int)GetPrivateProfileInt("Tyres", "PosX", 0, ini_file) + screenCenter;
	tyresWidget.position.y = GetPrivateProfileInt("Tyres", "PosY", tyresWidget.position.y, ini_file);
	tyresWidget.mode = GetPrivateProfileInt("Tyres", "Mode", 1, ini_file);
	tyresWidget.interval = GetPrivateProfileInt("Tyres", "Interval", 3000, ini_file);
	tyresWidget.threshold = GetPrivateProfileInt("Tyres", "Threshold", 255, ini_file);
	tyresWidget.useHeatMap = GetPrivateProfileInt("Tyres", "UseHeatMap", true, ini_file);
	tyresWidget.heatMapMin = GetPrivateProfileInt("Tyres", "HeatMapMin", 30, ini_file);
	tyresWidget.heatMapMax = GetPrivateProfileInt("Tyres", "HeatMapMax", 300, ini_file);


	// [Brakes]
	brakesWidget.enabled = GetPrivateProfileInt("Brakes", "Enabled", true, ini_file);
	brakesWidget.position.x = (int)GetPrivateProfileInt("Brakes", "PosX", 0, ini_file) + screenCenter;
	brakesWidget.position.y = GetPrivateProfileInt("Brakes", "PosY", brakesWidget.position.y, ini_file);
	brakesWidget.useHeatMap = GetPrivateProfileInt("Brakes", "UseHeatMap", true, ini_file);
	brakesWidget.heatMapMin = GetPrivateProfileInt("Brakes", "HeatMapMin", 0, ini_file);
	brakesWidget.heatMapMax = GetPrivateProfileInt("Brakes", "HeatMapMax", 1000, ini_file);


	// [Engine]
	engineWidget.enabled = GetPrivateProfileInt("engine", "Enabled", true, ini_file);
	engineWidget.position.x = (int)GetPrivateProfileInt("engine", "PosX", 0, ini_file) + screenCenter;
	engineWidget.position.y = GetPrivateProfileInt("engine", "PosY", engineWidget.position.y, ini_file);


	// [Rain]
	rainWidget.enabled = GetPrivateProfileInt("Rain", "Enabled", true, ini_file);
	rainWidget.position.x = (int)GetPrivateProfileInt("Rain", "PosX", 0, ini_file) + screenCenter;
	rainWidget.position.y = GetPrivateProfileInt("Rain", "PosY", rainWidget.position.y, ini_file);
	rainWidget.mode = GetPrivateProfileInt("Rain", "Mode", 1, ini_file);
	rainWidget.interval = GetPrivateProfileInt("Rain", "Interval", 3000, ini_file);


	// [Inputs]
	inputsWidget.enabled = GetPrivateProfileInt("Inputs", "Enabled", true, ini_file);
	inputsWidget.showFilteredInputs = GetPrivateProfileInt("Inputs", "FilteredInputs", false, ini_file);
	inputsWidget.position.x = (int)GetPrivateProfileInt("Inputs", "PosX", 0, ini_file) + screenCenter;
	inputsWidget.position.y = GetPrivateProfileInt("Inputs", "PosY", inputsWidget.position.y, ini_file);


	// [FPSMeter]
	fpsWidget.enabled = GetPrivateProfileInt("FPS", "Enabled", true, ini_file);
	fpsWidget.position.x = (int)GetPrivateProfileInt("FPS", "PosX", 0, ini_file) + screenCenter;
	fpsWidget.position.y = GetPrivateProfileInt("FPS", "PosY", fpsWidget.position.y, ini_file);


	// [StartingLights]
	lightsWidget.enabled = GetPrivateProfileInt("StartingLights", "Enabled", true, ini_file);
	lightsWidget.position.x = (int)GetPrivateProfileInt("StartingLights", "PosX", 0, ini_file) + screenCenter;
	lightsWidget.position.y = GetPrivateProfileInt("StartingLights", "PosY", lightsWidget.position.y, ini_file);


	// [Gear]
	gearWidget.enabled = GetPrivateProfileInt("Gear", "Enabled", true, ini_file);
	gearWidget.position.x = (int)GetPrivateProfileInt("Gear", "PosX", 0, ini_file) + screenCenter;
	gearWidget.position.y = GetPrivateProfileInt("Gear", "PosY", gearWidget.position.y, ini_file);


	// [Grid] section
	gridWidget.enabled = GetPrivateProfileInt("Grid", "Enabled", true, ini_file);
	gridWidget.position.x = (int)GetPrivateProfileInt("Grid", "PosX", 0, ini_file) + screenCenter;
	gridWidget.position.y = GetPrivateProfileInt("Grid", "PosY", gearWidget.position.y, ini_file);
	gridWidget.nDriversToShow = GetPrivateProfileInt("Grid", "Drivers", 3, ini_file);
	gridWidget.filterVehiclesInGarage = GetPrivateProfileInt("Grid", "FilterVehiclesInGarage", true, ini_file);
	gridWidget.PlayerOnTrackColor = GetPrivateProfileInt("Grid", "PlayerOnTrackColor", 0xFFd6a21f, ini_file);
	gridWidget.PlayerInPitsColor = GetPrivateProfileInt("Grid", "PlayerInPitsColor", 0xFFaa8119, ini_file);
	gridWidget.EqualCarOnTrackColor = GetPrivateProfileInt("Grid", "EqualCarOnTrackColor", 0xFFe6e6e6, ini_file);
	gridWidget.EqualCarInPitsColor = GetPrivateProfileInt("Grid", "EqualCarInPitsColor", 0xFF808080, ini_file);
	gridWidget.FasterCarOnTrackColor = GetPrivateProfileInt("Grid", "FasterCarOnTrackColor", 0xFFd95656, ini_file);
	gridWidget.FasterCarInPitsColor = GetPrivateProfileInt("Grid", "FasterCarInPitsColor", 0xFF933a3a, ini_file);
	gridWidget.SlowerCarOnTrackColor = GetPrivateProfileInt("Grid", "SlowerCarOnTrackColor", 0xFF2c88ce, ini_file);
	gridWidget.SlowerCarInPitsColor = GetPrivateProfileInt("Grid", "SlowerCarInPitsColor", 0xFF1b547f, ini_file);


	// [ClassColors] section
	gridWidget.userClassColorKeys.clear();
	gridWidget.userClassColorValues.clear();
	char * pBigString = new char[1024];
	DWORD dw = GetPrivateProfileSection("ClassColors", pBigString, 1024, ini_file);
	for (LPCSTR pToken = pBigString; pToken && *pToken; pToken = NextToken(pToken))
	{
		string str = pToken;
		size_t breakpos = str.find("=");
		string keySubString = str.substr(0, breakpos);
		char key[1024];
		strncpy(key, keySubString.c_str(), sizeof(key));
		D3DCOLOR value = GetPrivateProfileInt("ClassColors", key, 0xFFFFFFFF, ini_file);

		gridWidget.userClassColorKeys.push_back(key);
		gridWidget.userClassColorValues.push_back(value);
	}


	// [VehicleClasses] section
	gridWidget.userClassKeys.clear();
	gridWidget.userClassValues.clear();
	char * pBigString2 = new char[1024];
	DWORD dw2 = GetPrivateProfileSection("VehicleClasses", pBigString2, 1024, ini_file);
	for (LPCSTR pToken = pBigString2; pToken && *pToken; pToken = NextToken(pToken))
	{
		string str = pToken;
		size_t breakpos = str.find("=");
		string keySubString = str.substr(0, breakpos);
		char key[1024];
		strncpy(key, keySubString.c_str(), sizeof(key));
		char value[1024];
		GetPrivateProfileString("VehicleClasses", key, "ERROR", value, 1024, ini_file);

		gridWidget.userClassKeys.push_back(key);
		gridWidget.userClassValues.push_back(value);
	}
}

void VHUD::SaveConfig(const char *ini_file)
{
	char buffer[32];

	// [Config]
	WritePrivateProfileString("Config", "Borders", fuelWidget.useBorder ? "1" : "0", ini_file);

	// [Fuel]
	WritePrivateProfileString("Fuel", "Enabled", fuelWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", fuelWidget.position.x - screenCenter);
	WritePrivateProfileString("Fuel", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", fuelWidget.position.y);
	WritePrivateProfileString("Fuel", "PosY", buffer, ini_file);

	// [Tyres]
	WritePrivateProfileString("Tyres", "Enabled", tyresWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", tyresWidget.position.x - screenCenter);
	WritePrivateProfileString("Tyres", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", tyresWidget.position.y);
	WritePrivateProfileString("Tyres", "PosY", buffer, ini_file);

	// [Brakes]
	WritePrivateProfileString("Brakes", "Enabled", brakesWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", brakesWidget.position.x - screenCenter);
	WritePrivateProfileString("Brakes", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", brakesWidget.position.y);
	WritePrivateProfileString("Brakes", "PosY", buffer, ini_file);

	// [Engine]
	WritePrivateProfileString("Engine", "Enabled", engineWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", engineWidget.position.x - screenCenter);
	WritePrivateProfileString("Engine", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", engineWidget.position.y);
	WritePrivateProfileString("Engine", "PosY", buffer, ini_file);

	// [Rain]
	WritePrivateProfileString("Rain", "Enabled", rainWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", rainWidget.position.x - screenCenter);
	WritePrivateProfileString("Rain", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", rainWidget.position.y);
	WritePrivateProfileString("Rain", "PosY", buffer, ini_file);

	// [Inputs]
	WritePrivateProfileString("Inputs", "Enabled", inputsWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", inputsWidget.position.x - screenCenter);
	WritePrivateProfileString("Inputs", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", inputsWidget.position.y);
	WritePrivateProfileString("Inputs", "PosY", buffer, ini_file);

	// [FPSMeter]
	WritePrivateProfileString("FPS", "Enabled", fpsWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", fpsWidget.position.x - screenCenter);
	WritePrivateProfileString("FPS", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", fpsWidget.position.y);
	WritePrivateProfileString("FPS", "PosY", buffer, ini_file);

	// [StartingLights]
	WritePrivateProfileString("StartingLights", "Enabled", lightsWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", lightsWidget.position.x - screenCenter);
	WritePrivateProfileString("StartingLights", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", lightsWidget.position.y);
	WritePrivateProfileString("StartingLights", "PosY", buffer, ini_file);

	// [Gear]
	WritePrivateProfileString("Gear", "Enabled", gearWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", gearWidget.position.x - screenCenter);
	WritePrivateProfileString("Gear", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", gearWidget.position.y);
	WritePrivateProfileString("Gear", "PosY", buffer, ini_file);

	// [Grid]
	WritePrivateProfileString("Grid", "Enabled", gridWidget.enabled ? "1" : "0", ini_file);
	sprintf(buffer, " %.0f", gridWidget.position.x - screenCenter);
	WritePrivateProfileString("Grid", "PosX", buffer, ini_file);
	sprintf(buffer, " %.0f", gridWidget.position.y);
	WritePrivateProfileString("Grid", "PosY", buffer, ini_file);
}

