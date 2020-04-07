/**
* 2020.4.5 eastroot
*
* Heartbeat sensor
* detect heartbeat
*/

#pragma once

#include "Sensor.h"

class SHeartbeat : public Sensor
{
public:
	SHeartbeat();
	~SHeartbeat();

	virtual bool Active();

private:

};