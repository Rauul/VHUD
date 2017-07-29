#pragma once

#include <iostream>
#include <string>

using namespace std;

class Driver {

public:
	Driver();

	Driver(char[32], char[32], short, double, double, double, double, double, double, bool, bool, unsigned short, bool);

	char driverName[32];
	char vehicleClass[32];
	short totalLaps;
	double lapDistance;
	double totalLapDistance;		// totalLaps + lapDistance
	double lastLapTime;
	double bestLapTime;
	double relativeDistance;		// Relative distance to the player
	double timeIntoLap;
	double estimatedLapTime;
	double timeBehindLeader;
	double relativeTime;
	bool inPits;
	bool isPlayer;
	unsigned short place;
	int placeInClass;
	int classColor;
	bool inGarageStall;

private:
};