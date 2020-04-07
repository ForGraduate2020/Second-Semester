/**
* 2020.4.7 eastroot
*
* IMU sensor
* detect fall down
*/

#pragma once

#include "Sensor.h"

struct Axis
{
	int x;
	int y;
	int z;
};

class SIMU : public Sensor
{
public:
	SIMU();
	~SIMU();

	virtual bool Active();

private:
	Axis OldGyro;	// last gyroscope value
	Axis OldAccel;	// last acceleration value
	Axis OldMagnet;	// last magnetic field value

};