/**
  2020.4.5 eastroot
*/

#include "SHeartbeat.h"

SHeartbeat::SHeartbeat()
{
  maxValue = 0;
  isPeak = false;
  beatMsec = 0;
}

SHeartbeat::~SHeartbeat()
{

}

bool SHeartbeat::UpdateMilli(unsigned long Delta)
{
  Milli += Delta;
  if (Milli > ActiveMilli)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool SHeartbeat::Active(int rawValue)
{
  bool result = false;

  rawValue *= (1000 / Milli);

  if (rawValue * 4L < maxValue)
  {
    maxValue = rawValue * 0.8;
  }
  if (rawValue > maxValue - (1000 / Milli))
  {
    if (rawValue > maxValue)
    {
      maxValue = rawValue;
    }
    if (isPeak == false)
    {
      int bpm = 60000 / beatMsec;
      beatMsec = 0;

      if (bpm > 80 || bpm < 120)
      {
        Danger++;
        if (Danger > CriticalDanger)
        {
          result = true;
          Danger = 0;
        }
      }
      
      //result = true;
    }
    isPeak = true;
  }
  else if (rawValue < maxValue - (3000 / Milli))
  {
    isPeak = false;
    maxValue -= (1000 / Milli);
  }

  beatMsec += Milli;
  Milli = 0;

  return result;
}

int SHeartbeat::getBPM()
{
  int bpm = 60000 / beatMsec;
  beatMsec = 0;
  return bpm;
}
