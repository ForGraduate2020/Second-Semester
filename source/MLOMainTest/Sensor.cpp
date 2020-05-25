/**
* 2020.4.5 eastroot
*/

#include "Sensor.h"
//#include "Communicator.h"

Sensor::Sensor()
{
	Pin = 0;
	Type = 0;
	Danger = 0;
	CriticalDanger = 0;

	Milli = 0;
	Micro = 0;
	ActiveMilli = 0;
	ActiveMicro = 0;
}

Sensor::~Sensor()
{

}

void Sensor::PlayMilli(int HWPin, unsigned long Milli, int Critical)
{
	Pin = HWPin;
	ActiveMilli = Milli;
	CriticalDanger = Critical;
}

void Sensor::PlayMicro(int HWPin, unsigned long Micro, int Critical)
{
	Pin = HWPin;
	ActiveMicro = Micro;
	CriticalDanger = Critical;
}

bool Sensor::UpdateMilli(unsigned long Delta)
{
	Milli += Delta;
  if (Milli > ActiveMilli)
  {
    Milli = 0;
    return true;
  }
  else 
  {
    return false;
  }

	/*if (Milli > ActiveMilli)
	{
		if (Active())
		{
			Danger++;
			if (Danger > CriticalDanger)
			{
				// critical situation. send alert to hub
				Danger = 0;
				//Communicator::GetInstance()->Alert(Type);
			}
		}
		else
		{
			if (Danger > 0)
			{
				Danger--;
			}
		}
		Milli = 0;
	}*/
}

bool Sensor::UpdateMicro(unsigned long Delta)
{
	Micro += Delta;
  if (Micro > ActiveMicro)
  {
    Micro = 0;
    return true;
  }
  else
  {
    return false;
  }

	/*if (Micro > ActiveMicro)
	{
		if (Active())
		{
			Danger++;
			if (Danger > CriticalDanger)
			{
				// critical situation. send alert to hub
				Danger = 0;
				//Communicator::GetInstance()->Alert(Type);
			}
		}
		else
		{
			if (Danger > 0)
			{
				Danger--;
			}
		}
		Micro = 0;
	}*/
}

bool Sensor::Active()
{
	return false;	// no meaning
}

bool Sensor::Active(int rawValue)
{
  return false;
}
