/**
* 2020.4.7 eastroot
*
* Sound sensor
* record void and sense decibel at the same time. 
*/

#pragma once

#include "Sensor.h"
#include <SD.h>

class SSound : public Sensor
{
public:
	SSound();
	~SSound();

	/**
	 * detect decibel
 	 */
  bool Initialize();

  /**
   * record
   */
  bool BeginRecord();
	void Record(int rawValue);
  void EndRecord();
  
  bool Sample(int rawValue);

	void MarkDanger(int Type);

private:
	bool IsDanger;	// last record has more than one danger situation

	// decibel
	unsigned int SampleMax;
	unsigned int SampleMin;

  File recFile;
  
  byte header[44] = 
  {
    0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45,
    0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x00, 0x0F, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00,
    0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00
  };

};
