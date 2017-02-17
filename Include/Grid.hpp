#pragma once

#include "InternalsPlugin.hpp"
#include "Defaults.hpp"
#include "Driver.hpp"
#include <d3dx9.h>
#include <vector>
#include <algorithm>

#if _WIN64
#define GRID_ICON				"Bin64\\Plugins\\VHUD\\Images\\Grid.png"
#else
#define GRID_ICON				"Bin32\\Plugins\\VHUD\\Images\\Grid.png"
#endif

using namespace std;

class Grid
{
public:
	void StartSession(const ScoringInfoV01& info);
	void Init(const ScreenInfoV01& info);
	void Uninit(const ScreenInfoV01& info);
	void PreReset(const ScreenInfoV01& info);
	void PostReset(const ScreenInfoV01& info);
	void Update(const ScoringInfoV01 &info);
	void UpdatePosition();
	void Draw(bool inEditMode);
	void DrawBox(bool inEditMode);
	void DrawTxt();
	int TopGridPlace(int playerSlot);
	D3DCOLOR TextColor(int other, int player);

	RECT size = { 0, 0, 350, 194 };
	D3DXVECTOR3 position = { 0, 950, 0 };
	D3DCOLOR backgroundColor = 0xFF000000;
	D3DCOLOR borderColor = 0xFFAAAAAA;
	LPD3DXFONT bigFont = NULL;
	D3DXFONT_DESC bigFontDesc = { BIG_FONT_SIZE, 0, 1000, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXFONT smallFont = NULL;
	D3DXFONT_DESC smallFontDesc = { SMALL_FONT_SIZE, 0, 700, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, SMALL_FONT };
	LPD3DXSPRITE boxSprite = NULL;
	LPDIRECT3DTEXTURE9 boxTexture = NULL;
	vector<Driver> drivers;
	vector<string> vehicleClasses;
	vector<string> userClassColorKeys;
	vector<D3DCOLOR> userClassColorValues;
	bool useBorder = true;
	bool enabled = true;
	bool isMultiClass = false;
	bool isTimedSession = false;
	double currentTime = 0;
	double endTime = 0;
	short laps = 0;
	long maxLaps = 0;
	long session = 0;
	D3DCOLOR PlayerOnTrackColor = 0xFFd6a21f;
	D3DCOLOR PlayerInPitsColor = 0xFFaa8119;
	D3DCOLOR EqualCarOnTrackColor = 0xFFe6e6e6;
	D3DCOLOR EqualCarInPitsColor = 0xFF808080;
	D3DCOLOR FasterCarOnTrackColor = 0xFFd95656;
	D3DCOLOR FasterCarInPitsColor = 0xFF933a3a;
	D3DCOLOR SlowerCarOnTrackColor = 0xFF2c88ce;
	D3DCOLOR SlowerCarInPitsColor = 0xFF1b547f;
	D3DCOLOR CLASS_COLORS[100] = { 0xFFd9ba00, 0xFFb40ed9, 0xFF3da5a3, 0xFFf26d7d, 0xFF72f6ff, 0xFFc9d17f, 0xFF7ad893, 0xFFda6d2d, 0xFFc58516, 0xFFf3d9f2, 0xFFd32c10, 0xFF9d1d71,
		0xFF1c0bba, 0xFFe6589c, 0xFF0a3af6, 0xFF4cf556, 0xFF579b9b, 0xFFc06fac, 0xFFe53c26, 0xFF4e9440, 0xFF50ffec, 0xFFa9a16f, 0xFFd8e2a2, 0xFF46cee0, 0xFF750119, 0xFF16b873,
		0xFF073685, 0xFFb25f80, 0xFF67a6d0, 0xFFbd0288, 0xFF7d7149, 0xFF699d1f, 0xFFf6738e, 0xFFc0773d, 0xFF7b3aa5, 0xFFfaa167, 0xFF76001f, 0xFFdb5210, 0xFF6fa381, 0xFF6ae8a0,
		0xFF834df6, 0xFF6f34af, 0xFFd5be24, 0xFFad6251, 0xFFa78fdd, 0xFF498419, 0xFF24eeba, 0xFFeca3b9, 0xFFc56498, 0xFFb46f6d, 0xFF3ebf3e, 0xFF4796e6, 0xFFd04343 };
};