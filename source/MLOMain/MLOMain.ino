// 2020. 3. 20 eastroot

#include <SD.h>

enum AlertType 
{
  HEART_BEAT,
  DECIBEL
}

// global pin setting
int micPin = 0;
int heartbeatPin = 1;

// decibel
bool decibelTrigger = false;
byte maxDecibel = 0;
byte lowDecibel = 256;  // analog read is 8bit
byte criticalDecibel = 50;

// heartbeat
const int delayMsec = 60;
const int criticalBPM = 65;
const int criticalDecibel = 500;
bool heartTrigger = true;

// for time
unsigned long oldMilli = 0;
unsigned long oldMicro = 0;
unsigned long timer = 0;

// record
File recFile;

// wav header
PROGMEM const byte header [44] =
{
  0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45,
  0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00,
  0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00
};

void WriteHeader()
{
  for (byte pos = 0; pos < sizeof(header); pos++)
  {
    recFile.write(pgm_read_byte(&header[pos]));
  }
}

void Finalize()
{
  // 4  : format chunk size = file size - 8
  // 24 : sample rate = file size - 44 / record time
  // 40 : data chunk size = file size - 44

  byte finalValue[4];
  unsigned long fileSize = myFile.size();
  unsigned long riffSize = fileSize - 8;
  unsigned long dataSize = fileSize - 44;
  unsigned long sampleRate = dataSize / 10;  // sample count / sec

  Serial.print("sample rate : ");
  Serial.println(sampleRate);

  // write format chunk size
  finalValue[0] = riffSize & 0xff;
  finalValue[1] = (riffSize >> 8) & 0xff;
  finalValue[2] = (riffSize >> 16) & 0xff;
  finalValue[3] = (riffSize >> 24) & 0xff;

  recFile.seek(4);
  recFile.write(finalValue, 4);

  // write data chunk size
  finalValue[0] = dataSize & 0xff;
  finalValue[1] = (dataSize >> 8) & 0xff;
  finalValue[2] = (dataSize >> 16) & 0xff;
  finalValue[3] = (dataSize >> 24) & 0xff;

  recFile.seek(40);
  recFile.write(finalValue, 4);

  // write sample rate
  finalValue[0] = sampleRate & 0xff;
  finalValue[1] = (sampleRate >> 8) & 0xff;
  finalValue[2] = (sampleRate >> 16) & 0xff;
  finalValue[3] = (sampleRate >> 24) & 0xff;

  recFile.seek(24);
  recFile.write(finalValue, 4);

  // close file
  recFile.close();
}

bool HeartbeatDetected(int IRSensorPin, int delayTime)
{
  static int maxValue = 0;
  static bool isPeak = false;
  int rawValue;
  bool result = false;

  rawValue = analogRead(IRSensorPin);
  rawValue *= (1000 / delayTime);

  if (rawValue * 4 < maxValue)  // reset
  {
    maxValue = rawValue * 0.8;
  }

  if (rawValue > maxValue - (1000 / delayTime))
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
  else if (rawValue < maxValue - (3000 / delayTime))
  {
    isPeak = false;
    maxValue -= (1000 / delayTime);
  }

  return result;
}

int MicInput(int decibelPin)
{
  int result;
  int rawValue = analogRead(decibelPin);

  // TODO : calculate real decibel value from raw value
  result = rawValue;

  return result;
}

void Alert(int type)
{
  // mark record file name

  // make alert packet
  switch(type)
  {
    case HEART_BEAT: // heartbeat
    
    break;
    
    case DECIBEL: // decibel
    break;
    
    default:
    break;
  }

  // send alert to hub
  
}

void setup()
{
  Serial.begin(9600);

  while(!Serial);

  if (!SD.begin(4))
  {
    Serial.println("SD Initialize failed!");
    while(1);
  }
  Serial.println("SD Initialize...");

  

  // initialize timer
  oldMilli = millis();
  oldMicro = micros();
}

// loop must run at least 8,000/sec
void loop()
{
  // save current time
  unsigned long currentMilli = millis();
  unsigned long currentMicro = micros();

  // record
  if (currentMicro - oldMicro > 75)
  {
    oldMicro = currentMicro;
  }

  // decibel detection
  if (decibelTrigger)
  {
    byte amp = 0;
    if (maxDecibel > lowDecibel)
    {
      amp = maxDecibel - lowDecibel;
      if (amp > criticalDecibel)
      {
        Alert(DECIBEL);
      }
    }
    decibelTrigger = false;
  }

  // heartbeat detection
  static int beatMsec = 0;
  int heartRateBPM = 0;
  int highBPMCount = 0;

  // every delayMSecms
  if (heartTrigger)
  {
    if (HeartbeatDetected(heartbeatPin, delayMsec))
    {
      if (beatMsec > 0)
      {
        heartRateBPM = 60000 / beatMsec;
      }

      Serial.print("heartbeat : ");
      Serial.print(beatMsec);
      Serial.print(", ");
      Serial.println(heartRateBPM);

      // TODO : danger branch
      // send alert to hub when BPM too high
      if (heartRateBPM > criticalBPM)
      {
        highBPMCount++;
        if (highBPMCount > 5)
        {
          Alert(HEART_BEAT);
        }
      }

      beatMsec = 0;
    }
    else
    {

    }
    heartTrigger = false;
    oldMilli = currentMilli;
  }

  // trigger on every 60ms
  if (currentMilli - oldMilli > delayMSec)
  {
    oldMilli = currentMilli;
    heartTrigger = true;
    
    decibelTrigger = true;
    maxDecibel = 0;
    lowDecibel = 256;
  }
}
