#pragma once

#include "InternalsPlugin.hpp"
#include <d3dx9.h>

class Fuel
{
public:
	void Update(const TelemInfoV01& info);

	Fuel(float posX, float posY, int width, int height, D3DCOLOR bgColor, D3DCOLOR brdColor, bool useBdr);

	RECT size;
	D3DXVECTOR3 position;
	D3DCOLOR backgroundColor, boarderColor;
	double quantity = 0.00;
	double usedPerLap[3];
	double lapTime[3];
	bool useBoarder;
	bool enabled = true;

private:

};