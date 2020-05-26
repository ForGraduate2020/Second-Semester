/**
* 2020.4.7 eastroot
*
* Sound sensor
* record void and sense decibel at the same time. 
*/

#pragma once

#include "Sensor.h"

class SSound : public Sensor
{
public:
	SSound();
	~SSound();

  virtual bool UpdateMilli(unsigned long Delta);
	/**
	 * detect decibel
 	 */
  bool Initialize();
  
  bool Sample(int rawValue);
  unsigned int GetPTP(int rawValue);

	void MarkDanger(int Type);

private:
	bool IsDanger;	// last record has more than one danger situation

	// decibel
	unsigned int SampleMax;
	unsigned int SampleMin;

  int oldRaw;

  bool High;

};
