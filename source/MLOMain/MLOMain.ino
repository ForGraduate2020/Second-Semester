// 2020. 3. 20 eastroot

#include <SD.h>

// global pin setting
int micPin = 0;
int heartbeatPin = 1;

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
File record;

// wav header
PROGMEM const byte header [44] =
{
  0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45,
  0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00,
  0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00
};

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
    //Serial.println(currentMicro - oldMicro);
    oldMicro = currentMicro;
  }

  // decibel detection

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

  if (!heartTrigger && currentMilli - oldMilli > 60)
  {
    oldMilli = currentMilli;
    heartTrigger = true;
  }
}
