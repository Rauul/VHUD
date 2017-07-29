#include "Driver.hpp"

Driver::Driver()
{
	totalLaps = 0;
	lapDistance = 0.0;
	timeIntoLap = 0.0;
	estimatedLapTime = 0.0;
}

Driver::Driver(char pDriverName[32], char pVehicleClass[32], short pTotalLaps, double pLapDistance, double pFullLapDistance, double pBestLapTime, double pLastLapTime, double pTimeIntoLap,
	double pEstimatedLapTime, bool pInPits, bool pIsPlayer, unsigned short pPlace, bool pInGarageStall)
{
	for (int i = 0; i < 32; i++)
	{
		driverName[i] = pDriverName[i];
		vehicleClass[i] = pVehicleClass[i];
	}
	totalLaps = pTotalLaps;
	lapDistance = pLapDistance;
	totalLapDistance = pTotalLaps + (pLapDistance / pFullLapDistance);
	lastLapTime = pLastLapTime;
	bestLapTime = pBestLapTime;
	timeIntoLap = pTimeIntoLap;
	estimatedLapTime = pEstimatedLapTime;
	inPits = pInPits;
	isPlayer = pIsPlayer;
	place = pPlace;
	inGarageStall = pInGarageStall;
}