/**
* 2020.4.5 eastroot
*/

#include "SIMU.h"

SIMU::SIMU()
{

}

SIMU::~SIMU()
{

}

bool SIMU::Active()
{
	Axis Gyro;
	Axis Accel;
	Axis Magnet;
	bool result = false;

	// read 3 gyroscope value
	Gyro.x = 0;
	Gyro.y = 0;
	Gyro.z = 0;

	// read 3 acceleration value
	Accel.x = 0;
	Accel.y = 0;
	Accel.z = 0;

	// read 3 magnetic field value
	Magnet.x = 0;
	Magnet.y = 0;
	Magnet.z = 0;

	// calculate fall down

	// save current value
	OldGyro = Gyro;
	OldAccel = Accel;
	OldMagnet = Magnet;

	return result;
}