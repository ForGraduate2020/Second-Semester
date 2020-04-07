/**
* 2020.4.7 eastroot
*
* Sound sensor
* record void and sense decibel at the same time. 
*/

#pragma once

#include "Sensor.h"
// #include <SD.h>

class SSound : public Sensor
{
public:
	SSound();
	~SSound();

	virtual void UpdateMilli(unsigned long Delta);
	virtual void UpdateMicro(unsigned long Micro);

	/**
	* detect decibel
	*/
	virtual bool Active();

	void Record();

	void MarkDanger(int Type);

private:
	bool IsDanger;	// last record has more than one danger situation

	/**
	* decide to send old or next record file too or not
	*/
	// bool IsTooLate	
	// bool IsTooEarly	

	// decibel
	unsigned int SampleMax;
	unsigned int SampleMin;

	// record
	unsigned long RecordIndex;

	unsigned long RecordTime;

};