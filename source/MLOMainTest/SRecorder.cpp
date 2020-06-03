/**
  2020.4.5 eastroot
*/

#include "SRecorder.h"

SSound::SSound()
{
  IsDanger = false;
  
  SampleMax = 0;
  SampleMin = 5000;

  ActiveMicro = 125;	// default 1/8000sec for 8kHz

  oldRaw = 0;

  High = false;
}

SSound::~SSound()
{

}

bool SSound::UpdateMilli(unsigned long Delta)
{
  if (!High)
  {
    return false;
  }
  
  Milli += Delta;
  if (Milli > ActiveMilli)
  {
    Milli = 0;
    High = false;
    return true;
  }
  else
  {
    return false;
  }
}

bool SSound::Sample(int rawValue)
{
  bool result = false;
  unsigned int PeakToPeak;

  if (rawValue > SampleMax)
  {
    SampleMax = rawValue;
  }

  if (rawValue < SampleMin)
  {
    SampleMin = rawValue;
  }

  PeakToPeak = SampleMax - SampleMin;

  if (PeakToPeak > 100)
  {
    SampleMax = 0;
    SampleMin = 5000;
    result = true;
    High = true;
    Milli = 0;
  }

  return result;

}

void SSound::MarkDanger(int Type)
{
  IsDanger = true;
}
