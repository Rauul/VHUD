//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Header File                                   ﬁ
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

#ifndef _INTERNALS_EXAMPLE_H
#define _INTERNALS_EXAMPLE_H

#include "InternalsPlugin.hpp"
#include "Fuel.hpp"
#include <d3dx9.h>


#if _WIN64
#define LOG_FILE				"Bin64\\Plugins\\RelativeTime.log"
#define CONFIG_FILE				"Bin64\\Plugins\\RelativeTime.ini"
#define TEXTURE_BACKGROUND		"Bin64\\Plugins\\RelativeTimeBackground.png"
#define MOUSE_TEXTURE			"Bin64\\Plugins\\RelativeTimeCursor.png"
#else
#define LOG_FILE				"Bin32\\Plugins\\RelativeTime.log"
#define CONFIG_FILE				"Bin32\\Plugins\\RelativeTime.ini"
#define TEXTURE_BACKGROUND		"Bin32\\Plugins\\RelativeTimeBackground.png"
#define MOUSE_TEXTURE			"Bin32\\Plugins\\RelativeTimeCursor.png"
#endif


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

	void StartSession();           // session has started
	void EndSession();             // session has ended

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


	// USER CLASSES
	

	// USER FUNCTIONS
	void DrawBox(float posX, float posY, int width, int height, D3DCOLOR backgroundColor, D3DCOLOR boarderColor, bool useBoarder);
	void UpdateFuel(const TelemInfoV01& tinfo, const ScoringInfoV01& sinfo);
	bool NewLapStarted(const TelemInfoV01& tinfo, const ScoringInfoV01& sinfo);

	// USER VARIABELS
	bool inRealtime = false;
	
	LPD3DXSPRITE sprite = NULL;
	LPDIRECT3DTEXTURE9 texture = NULL;

private:

	void DrawGraphics(const ScreenInfoV01& info);
	double mET;  // needed for the hardware example
	bool mEnabled; // needed for the hardware example
};

#endif // _INTERNALS_EXAMPLE_H

