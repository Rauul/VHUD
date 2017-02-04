#pragma once

#include <d3dx9.h>

class Fuel
{
public:

	Fuel(float posX, float posY, int width, int height, D3DCOLOR bgColor, D3DCOLOR brdColor, bool useBdr);

	RECT size;
	D3DXVECTOR3 position;
	D3DCOLOR backgroundColor, boarderColor;
	bool useBoarder;
	double fuel;

private:

};