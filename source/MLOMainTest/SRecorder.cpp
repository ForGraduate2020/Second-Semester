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
}

SSound::~SSound()
{

}

bool SSound::Initialize()
{
  bool result = true;

  result = SD.begin(4);

  return result;
}

bool SSound::BeginRecord()
{
  bool result = true;
  
  if (SD.exists("rec.wav"))
  {
    SD.remove("rec.wav");
  }

  recFile = SD.open("rec.wav", O_WRITE | O_READ | O_CREAT);
  if (!recFile)
  {
    result = false;
  }
  
  for (byte pos = 0; pos < sizeof(header); pos++)
  {
    recFile.write(pgm_read_byte(&header[pos]));
  }

  return result;
}

void SSound::Record(int rawValue)
{
  recFile.write(rawValue);
}

void SSound::EndRecord()
{
  // 4  : format chunk size = file size - 8
  // 24 : sample rate = file size - 44 / record time
  // 40 : data chunk size = file size - 44

  byte finalValue[4];
  unsigned long fileSize = recFile.size();
  unsigned long riffSize = fileSize - 8;
  unsigned long dataSize = fileSize - 44;
  unsigned long sampleRate = dataSize / 20;  // sample count / sec

  Serial.print("sample rate : ");
  Serial.println(sampleRate);

  // write format chunk size
  finalValue[0] = riffSize & 0xff;
  finalValue[1] = (riffSize >> 8) & 0xff;
  finalValue[2] = (riffSize >> 16) & 0xff;
  finalValue[3] = (riffSize >> 24) & 0xff;
  // Is possible to make a fuction that returns an array of bytes?

  recFile.seek(4);
  recFile.write(finalValue, 4);

  // write data chunk size
  finalValue[0] = dataSize & 0xff;
  finalValue[1] = (dataSize >> 8) & 0xff;
  finalValue[2] = (dataSize >> 16) & 0xff;
  finalValue[3] = (dataSize >> 24) & 0xff;
  // Is possible to make a fuction that returns an array of bytes?

  recFile.seek(40);
  recFile.write(finalValue, 4);

  // write sample rate
  finalValue[0] = sampleRate & 0xff;
  finalValue[1] = (sampleRate >> 8) & 0xff;
  finalValue[2] = (sampleRate >> 16) & 0xff;
  finalValue[3] = (sampleRate >> 24) & 0xff;

  recFile.seek(24);
  recFile.write(finalValue, 4);

  // Check if already in little-endian order
  recFile.close();

  // send if it is danger file
  if (IsDanger)
  {
    // Communicator::GetInstance()->SendFile();
    IsDanger = false;
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

  if (PeakToPeak > 500)
  {
    SampleMax = 0;
    SampleMin = 5000;
    result = true;
  }

  return result;

}

void SSound::MarkDanger(int Type)
{
  IsDanger = true;
}
