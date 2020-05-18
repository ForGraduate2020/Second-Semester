/**
* 2020.4.5 eastroot
*
* Sensor
* all sensor's parent class 
* update by board(arduino or pi) code and active every active time 
*/

#pragma once

#include "MLOGlobal.h"

class Sensor
{
public:
	Sensor();
	~Sensor();

	/**
	* play milli sensor or micro sensor
	* 
	* <require>
	* HWPin > 0 && HWPin <= board pin max
	* ActiveMilli > 0 || ActiveMicro > 0
	* Critical > 0
	* 
	* <result>
	* set all value to run this sensor
	*/
	virtual void PlayMilli(int HWPin, unsigned long Milli, int Critical);
	virtual void PlayMicro(int HWPin, unsigned long Micro, int Critical);

	/**
	* Active
	* 
	* execute every specified time.
	* actual logic for HW input
	*/
  virtual bool Active();
	virtual bool Active(int rawValue);

	virtual bool UpdateMilli(unsigned long Delta);
	virtual bool UpdateMicro(unsigned long Delta);

protected:
	int Pin;
	int Type;	// sensor type (heartbeat, decibel, ...)
	int Danger;
	int CriticalDanger;

	int Milli;	// passed ms
	int Micro;	// passed us
	unsigned long ActiveMilli;	// ms required to read sensor
	unsigned long ActiveMicro;	// us required to read sensor

};
