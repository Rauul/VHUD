#pragma once

#ifndef _INTERNALS_EXAMPLE_H
#define _INTERNALS_EXAMPLE_H

#include "InternalsPlugin.hpp"
#include "Menu.hpp"
#include "Cursor.hpp"
#include "Fuel.hpp"
#include "Tyres.hpp"
#include "Engine.hpp"
#include "Rain.hpp"
#include "Inputs.hpp"
#include "FPSMeter.hpp"
#include "StartingLights.hpp"
#include "Gear.hpp"
#include "Grid.hpp"
#include "Brakes.hpp"
#include <d3dx9.h>
#include <sstream>
#include <Windows.h>

#define PLUGIN_NAME					"V-HUD"
#define PLUGIN_VERSION				"0.84b6"

#if _WIN64
#define CONFIG_FILE					"Bin64\\Plugins\\VHUD\\VHUD.ini"
#define BACKGROUND_TEXTURE			"Bin64\\Plugins\\VHUD\\Images\\Background.png"
#define SPLASH_TEXTURE				"Bin64\\Plugins\\VHUD\\Images\\Splash.png"
#else
#define CONFIG_FILE					"Bin32\\Plugins\\VHUD\\VHUD.ini"
#define BACKGROUND_TEXTURE			"Bin32\\Plugins\\VHUD\\Images\\Background.png"
#define SPLASH_TEXTURE				"Bin32\\Plugins\\VHUD\\Images\\Splash.png"
#endif

#define USE_BORDERS					true
#define BACKGROUND_COLOR			0xFF000000
#define BORDER_COLOR				0xFFAAAAAA

#define DEFAULT_EDIT_KEY			(0x45)      // "E"
#define DEFAULT_RESET_KEY			(0x52)		// "R"
#define KEY_DOWN(k)					((GetAsyncKeyState(k) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))

class VHUD : public InternalsPluginV06
{

public:

	// Constructor/destructor
	VHUD() {}
	~VHUD() {}

	// These are the functions derived from base class InternalsPlugin
	// that can be implemented.
	void Startup(long version);  // game startup
	void Shutdown();               // game shutdown

	void EnterRealtime();          // entering realtime
	void ExitRealtime();           // exiting realtime

	void StartSession(const ScoringInfoV01& info);           // session has started
	void EndSession();             // session has ended

	void Load();

	// GAME OUTPUT
	long WantsTelemetryUpdates() { return(1); } // CHANGE TO 1 TO ENABLE TELEMETRY EXAMPLE!
	void UpdateTelemetry(const TelemInfoV01& info);

	bool WantsScoringUpdates() { return(true); } // CHANGE TO TRUE TO ENABLE SCORING EXAMPLE!
	void UpdateScoring(const ScoringInfoV01& info);

	bool WantsGraphicsUpdates() { return(false); } // CHANGE TO TRUE TO ENABLE GRAPHICS EXAMPLE!
	void UpdateGraphics(const GraphicsInfoV02& info) {};

	// GAME INPUT
	bool HasHardwareInputs() { return(false); } // CHANGE TO TRUE TO ENABLE HARDWARE EXAMPLE!
	void UpdateHardware(const double fDT) { mET += fDT; } // update the hardware with the time between frames
	void EnableHardware() { mEnabled = true; }             // message from game to enable hardware
	void DisableHardware() { mEnabled = false; }           // message from game to disable hardware

	// See if the plugin wants to take over a hardware control.  If the plugin takes over the
	// control, this method returns true and sets the value of the double pointed to by the
	// second arg.  Otherwise, it returns false and leaves the double unmodified.
	bool CheckHWControl(const char* const controlName, float& fRetVal);
	bool ForceFeedback(double &forceValue) { return(false); }  // SEE FUNCTION BODY TO ENABLE FORCE EXAMPLE

	// SCREEN NOTIFICATIONS
	void InitScreen(const ScreenInfoV01& info);                  // Now happens right after graphics device initialization
	void UninitScreen(const ScreenInfoV01& info);                // Now happens right before graphics device uninitialization

	void RenderScreenBeforeOverlays(const ScreenInfoV01& info);  // before rFactor overlays

	void PreReset(const ScreenInfoV01& info);					 // after detecting device lost but before resetting
	void PostReset(const ScreenInfoV01& info);					 // after resetting


	

	// USER FUNCTIONS
	bool MouseIsOver(Menu);
	bool MouseIsOver(Fuel);
	bool MouseIsOver(Tyres);
	bool MouseIsOver(Engine);
	bool MouseIsOver(Rain);
	bool MouseIsOver(Inputs);
	bool MouseIsOver(FPSMeter); 
	bool MouseIsOver(StartingLights);
	bool MouseIsOver(Gear);
	bool MouseIsOver(Grid);
	bool MouseIsOver(Brakes);
	bool IsPlayer(const ScoringInfoV01& info);
	void UpdatePositions();
	void MenuEvents();
	void ResetPositions(const ScreenInfoV01& info);
	void DrawSplashScreen();
	void LoadConfig(const char *ini_file);
	void SaveConfig(const char *ini_file);

	// USER VARIABELS
	int editKey = DEFAULT_EDIT_KEY;
	int resetKey = DEFAULT_RESET_KEY;
	long playerSlot = 0;
	float screenCenter = 0;
	float screenHeight = 0;
	bool inRealtime = false;
	bool inEditMode = false;
	bool editkeyDownLastFrame = false;
	bool resetkeyDownLastFrame = false;
	bool mouseDownLastFrame = false;
	bool isPlayer = false;
	bool displayedWelcomeMessage = false;
	float scaleFactor = 0;
	
	DWORD timeLoaded = GetTickCount();
	LPD3DXSPRITE splashSprite = NULL;
	LPDIRECT3DTEXTURE9 splashTexture = NULL;

private:
	void DrawGraphics(const ScreenInfoV01& info);
	bool WantsToDisplayMessage(MessageInfoV01 & msgInfo);
	double mET;  // needed for the hardware example
	bool mEnabled; // needed for the hardware example
};

#endif // _INTERNALS_EXAMPLE_H
