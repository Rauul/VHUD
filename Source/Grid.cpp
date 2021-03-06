#include "Grid.hpp"
#include "VHUD.hpp"
#include <stdio.h>

void Grid::StartSession(const ScoringInfoV01 & info)
{
	maxLaps = info.mMaxLaps;
	endTime = info.mEndET;
	isTimedSession = maxLaps > 99999;
}

void Grid::Init(const ScreenInfoV01 & info)
{
	GetPrivateProfileString("Config", "Font", "Tahoma", smallFontDesc.FaceName, 32, CONFIG_FILE);
	smallFontDesc.Height = GetPrivateProfileInt("Config", "SmallFontSize", 20, CONFIG_FILE);
	GetPrivateProfileString("Config", "Font", "Tahoma", bigFontDesc.FaceName, 32, CONFIG_FILE);
	bigFontDesc.Height = GetPrivateProfileInt("Config", "BigFontSize", 40, CONFIG_FILE);

	D3DXCreateSprite((LPDIRECT3DDEVICE9)info.mDevice, &boxSprite);
	D3DXCreateTextureFromFile((LPDIRECT3DDEVICE9)info.mDevice, BACKGROUND_TEXTURE, &boxTexture);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &bigFontDesc, &bigFont);
	D3DXCreateFontIndirect((LPDIRECT3DDEVICE9)info.mDevice, &smallFontDesc, &smallFont);

	size.bottom = nDriversToShow * 48 + 24 + 26;
}

void Grid::Uninit(const ScreenInfoV01 & info)
{
	if (boxTexture) {
		boxTexture->Release();
		boxTexture = NULL;
	}
	if (boxSprite) {
		boxSprite->Release();
		boxSprite = NULL;
	}
	if (bigFont) {
		bigFont->Release();
		bigFont = NULL;
	}
	if (smallFont) {
		smallFont->Release();
		smallFont = NULL;
	}
}

void Grid::PreReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnLostDevice();
	if (bigFont)
		bigFont->OnLostDevice();
	if (smallFont)
		smallFont->OnLostDevice();
}

void Grid::PostReset(const ScreenInfoV01 & info)
{
	if (boxSprite)
		boxSprite->OnResetDevice();
	if (bigFont)
		bigFont->OnResetDevice();
	if (smallFont)
		smallFont->OnResetDevice();
}

void Grid::Update(const ScoringInfoV01 & info)
{
	if (!enabled)
		return;

	isTimedSession = maxLaps > 99999;
	currentTime = info.mCurrentET;
	endTime = info.mEndET;
	maxLaps = info.mMaxLaps;
	
	session = info.mSession;
	int playerSlot;
	drivers.clear();

	// Fill the <drivers> vector
	for (long l = 0; l < info.mNumVehicles; l++)
	{
		VehicleScoringInfoV01 &vinfo = info.mVehicle[l];

		Driver driver(vinfo.mDriverName, vinfo.mVehicleClass, vinfo.mTotalLaps, vinfo.mLapDist, info.mLapDist, vinfo.mBestLapTime, vinfo.mLastLapTime, vinfo.mTimeIntoLap, vinfo.mEstimatedLapTime, vinfo.mInPits, vinfo.mIsPlayer, vinfo.mPlace, vinfo.mInGarageStall);
		if (vinfo.mIsPlayer)
		{
			playerSlot = (int)drivers.size();
			laps = driver.totalLaps + 1;
		}

		drivers.push_back(driver);
	}

	// Calculate the relative distance to the player
	for (int i = 0; i < (int)drivers.size(); i++)
	{
		if (drivers[i].isPlayer)
		{
			drivers[i].relativeDistance = 0.0;
			drivers[i].relativeTime = 0.0;
			continue;
		}

		// If the vehicle is on the first half of the lap
		if (drivers[playerSlot].lapDistance < info.mLapDist / 2)
		{
			if (drivers[i].lapDistance < drivers[playerSlot].lapDistance || drivers[i].lapDistance < drivers[playerSlot].lapDistance + info.mLapDist / 2)
				drivers[i].relativeDistance = drivers[playerSlot].lapDistance - drivers[i].lapDistance;
			else
				drivers[i].relativeDistance = info.mLapDist - drivers[i].lapDistance + drivers[playerSlot].lapDistance;
		}
		else
			if (drivers[i].lapDistance > drivers[playerSlot].lapDistance || drivers[i].lapDistance > drivers[playerSlot].lapDistance - info.mLapDist / 2)
			{
				drivers[i].relativeDistance = drivers[playerSlot].lapDistance - drivers[i].lapDistance;
			}
			else
			{
				drivers[i].relativeDistance = -abs(drivers[playerSlot].lapDistance - info.mLapDist - drivers[i].lapDistance);
			}


		// Calculate the reative time to driver
		if (drivers[playerSlot].timeIntoLap < drivers[playerSlot].estimatedLapTime / 2)
		{
			if (drivers[i].timeIntoLap < drivers[playerSlot].timeIntoLap || drivers[i].timeIntoLap < drivers[playerSlot].timeIntoLap + drivers[i].estimatedLapTime / 2)
				drivers[i].relativeTime = drivers[playerSlot].timeIntoLap - drivers[i].timeIntoLap;
			else
				drivers[i].relativeTime = drivers[i].estimatedLapTime - drivers[i].timeIntoLap + drivers[playerSlot].timeIntoLap;
		}
		else
			if (drivers[i].timeIntoLap > drivers[playerSlot].timeIntoLap || drivers[i].timeIntoLap > drivers[playerSlot].timeIntoLap - drivers[i].estimatedLapTime / 2)
			{
				drivers[i].relativeTime = drivers[playerSlot].timeIntoLap - drivers[i].timeIntoLap;
			}
			else
			{
				drivers[i].relativeTime = -abs(drivers[playerSlot].timeIntoLap - drivers[playerSlot].estimatedLapTime - drivers[i].timeIntoLap);
			}
	}

	// Create class vectors
	vehicleClasses.clear();
	for (int i = 0; i < (int)drivers.size(); i++)
	{
		for (int j = 0; j < (int)userClassValues.size(); j++)
		{
			if (userClassValues[j].find(drivers[i].vehicleClass) != string::npos) 
			{
				sprintf(drivers[i].vehicleClass, "%s", userClassKeys[j]);
				break;
			}
		}

		if (find(vehicleClasses.begin(), vehicleClasses.end(), drivers[i].vehicleClass) == vehicleClasses.end())
			vehicleClasses.push_back(drivers[i].vehicleClass);
	}

	isMultiClass = vehicleClasses.size() > 1;
	sort(drivers.begin(), drivers.end(), [](const Driver& lhs, const Driver& rhs) { return lhs.place < rhs.place; });

	for (int i = 0; i < (int)vehicleClasses.size(); i++)
	{
		int placeInClass = 1;
		for (int j = 0; j < (int)drivers.size(); j++)
		{
			if (drivers[j].vehicleClass == vehicleClasses[i])
			{
				drivers[j].placeInClass = placeInClass;
				drivers[j].classColor = i;
				placeInClass++;
			}
		}
	}

	for (int i = 0; i < (int)drivers.size(); i++)
	{
		if (bestLapTimeSession <= 0 || 
			(drivers[i].bestLapTimePersonal > 0 && drivers[i].bestLapTimePersonal < bestLapTimeSession))
			bestLapTimeSession = drivers[i].bestLapTimePersonal;
	}

	if (filterVehiclesInGarage)
	{
		for (int i = 0; i < (int)drivers.size(); i++)
		{
			if (drivers[i].inGarageStall && !drivers[i].isPlayer)
			{
				drivers.erase(drivers.begin() + i);
				i--;
			}
		}
	}

	sort(drivers.begin(), drivers.end(), [](const Driver& lhs, const Driver& rhs) { return lhs.relativeTime < rhs.relativeTime; });
	
}

void Grid::UpdatePosition()
{
	if (!enabled)
		return;

	POINT cursorPosition;
	if (GetCursorPos(&cursorPosition))
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			position.x = cursorPosition.x - size.right / 2;
			position.y = cursorPosition.y - size.bottom / 2;
		}

		else if (GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			position.x = RoundNum(cursorPosition.x, 10) - size.right / 2;
			position.y = RoundNum(cursorPosition.y, 10) - size.bottom / 2;
		}
		else
		{
			position.x = RoundNum(cursorPosition.x, 5) - size.right / 2;
			position.y = RoundNum(cursorPosition.y, 5) - size.bottom / 2;
		}
	}
}

long Grid::RoundNum(long num, long multiple)
{
	long rem = num % multiple;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
}

void Grid::Draw(bool inEditMode)
{
	if (!enabled)
		return;

	DrawBox(inEditMode);
	DrawTxt();
}

void Grid::DrawBox(bool inEditMode)
{
	boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
	boxSprite->Draw(boxTexture, &size, NULL, &position, backgroundColor);
	boxSprite->End();

	if (useBorder || inEditMode)
	{
		RECT borderSize = size;
		D3DXVECTOR3 borderPosition = position;

		// Top
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.bottom = 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Bottom
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.y += size.bottom - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Left
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderSize.right = 1;
		borderSize.bottom = size.bottom - 2;
		borderPosition.y = position.y + 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();

		// Right
		boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
		borderPosition.x += size.right - 1;
		boxSprite->Draw(boxTexture, &borderSize, NULL, &borderPosition, borderColor);
		boxSprite->End();
	}
}

void Grid::DrawTxt()
{
	RECT placePosition, classBoxRect, placeInClassPosition, namePosition, timePosition, timerPosition;

	placePosition.left = position.x + 11;
	placePosition.top = position.y + 6;
	placePosition.right = placePosition.left + 20;
	placePosition.bottom = placePosition.top + 20;

	classBoxRect.left = placePosition.left + 28;
	classBoxRect.top = placePosition.top;
	classBoxRect.right = classBoxRect.left + 32;
	classBoxRect.bottom = classBoxRect.top + 19;

	placeInClassPosition.left = classBoxRect.left + 10;
	placeInClassPosition.top = placePosition.top;
	placeInClassPosition.right = placeInClassPosition.left + 20;
	placeInClassPosition.bottom = placeInClassPosition.top + 20;

	namePosition.left = placeInClassPosition.left + 26;
	namePosition.top = placePosition.top;
	namePosition.right = namePosition.left + 200;
	namePosition.bottom = namePosition.top + 20;

	timePosition.left = namePosition.right;
	timePosition.top = placePosition.top;
	timePosition.right = timePosition.left + 67;
	timePosition.bottom = timePosition.top + 20;

	timerPosition.left = placePosition.left;
	timerPosition.top = position.y + size.bottom - 26;
	timerPosition.right = timePosition.right;
	timerPosition.bottom = timerPosition.top + 26;

	if (!isMultiClass)
		namePosition.left -= 30;

	D3DXVECTOR3 classBoxPos = { 0,0,0 };
	classBoxPos.x = classBoxRect.left;
	classBoxPos.y = classBoxRect.top;


	int playerSlot = 0;
	for (int i = 0; i < (int)drivers.size(); i++)
	{
		if (drivers[i].isPlayer)
		{
			playerSlot = i;
			break;
		}
	}

	int gridStartNum = TopGridPlace(playerSlot);

	for (int i = 0; i < nDriversToShow * 2 + 1; i++)
	{
		if (gridStartNum + i >= (int)drivers.size())
			break;

		D3DCOLOR color = TextColor(gridStartNum + i, playerSlot);
		bool isPlayer = drivers[gridStartNum + i].isPlayer;

		char c_buffer[32] = "";
		sprintf(c_buffer, "%d", drivers[gridStartNum + i].place);
		smallFont->DrawText(NULL, c_buffer, -1, &placePosition, DT_RIGHT, color);

		if (isMultiClass)
		{
			D3DCOLOR classColor = 0xFFFFFFFF;
			string currVehicleClass = drivers[gridStartNum + i].vehicleClass;

			classColor = CLASS_COLORS[drivers[gridStartNum + i].classColor];

			if (std::find(userClassColorKeys.begin(), userClassColorKeys.end(), drivers[gridStartNum + i].vehicleClass) != userClassColorKeys.end())
			{
				for (int i = 0; i < userClassColorKeys.size(); i++)
				{
					if (userClassColorKeys[i] == currVehicleClass)
					{
						classColor = userClassColorValues[i];
						break;
					}
				}
			}

			boxSprite->Begin(D3DXSPRITE_ALPHABLEND);
			boxSprite->Draw(boxTexture, &classBoxRect, NULL, &classBoxPos, classColor);
			boxSprite->End();

			sprintf(c_buffer, "%d", drivers[gridStartNum + i].placeInClass);
			D3DCOLOR picColor = 0xFF000000;
			smallFont->DrawText(NULL, c_buffer, -1, &placeInClassPosition, DT_RIGHT, picColor);
		}

		smallFont->DrawText(NULL, drivers[gridStartNum + i].driverName, -1, &namePosition, DT_LEFT, color);

		if (drivers[gridStartNum + i].timeIntoLap < 10 && drivers[gridStartNum + i].lastLapTime > 0)
		{		
			D3DCOLOR flashColor = 0xFFFFFFFF;
			int totalms, minutes, seconds, ms;
			totalms = floor(((drivers[gridStartNum + i].lastLapTime * 10000) + 5) / 10);
			minutes = totalms / (60 * 1000);
			totalms = totalms - minutes*(60 * 1000);
			seconds = totalms / 1000;
			ms = totalms - seconds * 1000;

			if (drivers[gridStartNum + i].lastLapTime == drivers[gridStartNum + i].bestLapTimePersonal)
				flashColor = D3DCOLOR_ARGB(255, 0, 255, 0);

			if (drivers[gridStartNum + i].lastLapTime == bestLapTimeSession)
				flashColor = D3DCOLOR_ARGB(255, 255, 0, 255);

			sprintf(c_buffer, "%d:%02d.%03d", minutes, seconds, ms);
			smallFont->DrawText(NULL, c_buffer, -1, &timePosition, DT_RIGHT | DT_VCENTER, flashColor);
		}

		else 
		{

			if (isPlayer)
			{
				if (drivers[playerSlot].lastLapTime > 0)
				{
					//sprintf(c_buffer, "%s", "0.0");
					int totalms, minutes, seconds, ms;
					totalms = floor(((drivers[playerSlot].lastLapTime * 10000) + 5) / 10);
					minutes = totalms / (60 * 1000);
					totalms = totalms - minutes*(60 * 1000);
					seconds = totalms / 1000;
					ms = totalms - seconds * 1000;
					sprintf(c_buffer, "%d:%02d.%03d", minutes, seconds, ms);
				}
				else
					sprintf(c_buffer, "%s", "-:--.---");
			}
			else
				sprintf(c_buffer, "%.1f", drivers[gridStartNum + i].relativeTime * -1);
			smallFont->DrawText(NULL, c_buffer, -1, &timePosition, DT_RIGHT, color);
		}

			//// Adjust for next itteration
			placePosition.top += 23;
			placePosition.bottom += 23;

			classBoxRect.top = placePosition.top;
			classBoxRect.bottom = classBoxRect.top + 19;
			classBoxPos.x = classBoxRect.left;
			classBoxPos.y = classBoxRect.top;

			placeInClassPosition.top = placePosition.top;
			placeInClassPosition.bottom = placeInClassPosition.top + 23;

			namePosition.top = placePosition.top;
			namePosition.bottom = namePosition.top + 23;

			timePosition.top = placePosition.top;
			timePosition.bottom = timePosition.top + 23;
	}

	char c_buffer[32] = "";

	if (isTimedSession)
	{
		sprintf(c_buffer, "Lap:  %d", laps);
		smallFont->DrawText(NULL, c_buffer, -1, &timerPosition, DT_LEFT | DT_VCENTER, SecondaryTextColor);

		int total, hours, minutes, seconds;
		total = endTime - currentTime;
		if (total < 0) total = 0;
		minutes = total / 60;
		seconds = total % 60;
		hours = minutes / 60;
		minutes = minutes % 60;

		sprintf(c_buffer, "%2d:%02d:%02d", hours, minutes, seconds);
		smallFont->DrawText(NULL, c_buffer, -1, &timerPosition, DT_RIGHT | DT_VCENTER, SecondaryTextColor);
	}
	else
	{
		sprintf(c_buffer, "Lap:  %d / %d", laps, maxLaps);
		smallFont->DrawText(NULL, c_buffer, -1, &timerPosition, DT_LEFT | DT_VCENTER, SecondaryTextColor);

		int total, hours, minutes, seconds;
		total = currentTime;
		if (total < 0) total = 0;
		minutes = total / 60;
		seconds = total % 60;
		hours = minutes / 60;
		minutes = minutes % 60;

		sprintf(c_buffer, "%2d:%02d:%02d", hours, minutes, seconds);
		smallFont->DrawText(NULL, c_buffer, -1, &timerPosition, DT_RIGHT | DT_VCENTER, SecondaryTextColor);
	}

	time_t t = time(0);
	struct tm *now = localtime(&t);

	sprintf(c_buffer, "%02d:%02d", now->tm_hour, now->tm_min);
	smallFont->DrawText(NULL, c_buffer, -1, &timerPosition, DT_CENTER | DT_VCENTER, SecondaryTextColor);
}

double Grid::GetRelativeTimeToPlayer(Driver other, int playerSlot)
{
	return 0.0;
}

int Grid::TopGridPlace(int playerSlot)
{
	if ((int)drivers.size() <= nDriversToShow * 2 + 1)
		return 0;

	if (playerSlot < nDriversToShow + 1)
		return 0;

	if (playerSlot >= (int)drivers.size() - nDriversToShow)
		return (int)drivers.size() - (nDriversToShow * 2 + 1);

	return playerSlot - nDriversToShow;
}

D3DCOLOR Grid::TextColor(int other, int player)
{
	bool isPlayer = drivers[other].isPlayer;

	// if it's us, we want player text colors
	if (isPlayer)
	{
		if (drivers[other].inPits)
			return PlayerInPitsColor;
		return PlayerOnTrackColor;
	}

	if (session > 9)
	{
		// somthings up at the start of a race so lock to equal car colors
		if (drivers[player].totalLaps < 1)
		{
			if (drivers[other].inPits)
				return EqualCarInPitsColor;
			return EqualCarOnTrackColor;
		}

		// if a faster car is >.5 lap ahead of us, we want faster car colors
		if ((drivers[other].place < drivers[player].place && drivers[other].relativeTime > 0) ||
			(drivers[other].place < drivers[player].place && drivers[other].totalLapDistance > drivers[player].totalLapDistance + 1))
		{
			if (drivers[other].inPits)
				return FasterCarInPitsColor;
			return FasterCarOnTrackColor;
		}

		// else if a slower car is >.5 lap behind us, we want slower car colors
		else if ((drivers[other].place > drivers[player].place && drivers[other].relativeTime < 0) ||
			(drivers[other].place > drivers[player].place && drivers[other].totalLapDistance < drivers[player].totalLapDistance - 1))
		{
			if (drivers[other].inPits)
				return SlowerCarInPitsColor;
			return SlowerCarOnTrackColor;
		}

		// else we fight for position and want equal car colors
		else
		{
			if (drivers[other].inPits)
				return EqualCarInPitsColor;
			return EqualCarOnTrackColor;
		}
	}

	// if not in race we want white
	else
	{
		if (drivers[other].inPits)
			return EqualCarInPitsColor;
		return EqualCarOnTrackColor;
	}
}
