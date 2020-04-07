/**
* 2020.4.5 eastroot
*/

#include "SRecorder.h"

SSound::SSound()
{
	IsDanger = false;

	ActiveMicro = 125;	// default 1/8000sec for 8kHz 
}

SSound::~SSound()
{

}

void SSound::UpdateMilli(unsigned long Delta)
{
	Sensor::UpdateMilli(Delta);

	RecordTime += Delta;

	// every 10min
	if (RecordTime > 600000)
	{
		// finalize
		RecordTime = 0;
		// Record();
	}
}

void SSound::UpdateMicro(unsigned long Delta)
{
	unsigned int Sample;

	// get max min peek
	if (Sample > SampleMax)
	{
		SampleMax = Sample;
	}

	if (Sample < SampleMin)
	{
		SampleMin = Sample;
	}

	// record

}

bool SSound::Active()
{
	unsigned int PeakToPeak;
	bool result = false;

	PeakToPeak = SampleMax - SampleMin;

	// if (PeekToPeek > some value) result = true;

	return result;
}

void SSound::Record()
{
	// make file

	// initialize

	RecordIndex++;
}

void SSound::MarkDanger(int Type)
{
	IsDanger = true;
}