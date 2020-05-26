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


  virtual bool UpdateMilli(unsigned long Delta);
  int getMilli() { return Milli; };
	virtual bool Active(int rawValue);

  int getBPM();

private:
  int maxValue;
  bool isPeak;
  int beatMsec;

};
