/**
* 2020.4.5 eastroot
*/

#include "SHeartbeat.h"

SHeartbeat::SHeartbeat()
{

}

SHeartbeat::~SHeartbeat()
{

}

bool SHeartbeat::Active()
{
    int maxValue = 0;
    bool isPeak = false;
    int rawValue = 0;
    bool result = false;

    // rawValue = analogRead(Pin);
    rawValue *= (1000 / ActiveMilli);

    if (rawValue * 4 < maxValue)  // reset
    {
        maxValue = rawValue * 0.8;
    }

    if (rawValue > maxValue - (1000 / ActiveMilli))
    {
        if (rawValue > maxValue)
        {
            maxValue = rawValue;
        }
        if (!isPeak)
        {
            result = true;
        }
        isPeak = true;
    }
    else if (rawValue < maxValue - (3000 / ActiveMilli))
    {
        isPeak = false;
        maxValue -= (1000 / ActiveMilli);
    }

}