#include "Fuel.hpp"
#include "VHUD.hpp"

Fuel::Fuel(float posX, float posY, int width, int height, D3DCOLOR bgColor, D3DCOLOR brdColor, bool useBrd)
{
	position.x = posX;
	position.y = posY;
	position.z = 0;
	size.left = 0;
	size.top = 0;
	size.right = width;
	size.bottom = height;
	backgroundColor = bgColor;
	boarderColor = brdColor;
	useBoarder = useBrd;
}