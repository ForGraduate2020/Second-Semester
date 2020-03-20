// 2020. 3. 20 eastroot

// global pin setting
int micPin = 0;
int heartbeatPin = 1;

// global variable
const int delayMsec = 60;
const int criticalBPM = 65;
const int criticalDecibel = 500;

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
}

void loop() 
{
  // decibel detection
  if (MicInput(micPin) > criticalDecibel)
  {
    // TODO : send alert to hub 
    // mark time to recort DB 
  }

  // TODO : make global timer to control all sensors
  // I think delay is bad way
  // delay(100);
  
  // heartbeat detection
  static int beatMsec = 0;
  int heartRateBPM = 0;
  int highBPMCount = 0;

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

  delay(delayMsec);
  beatMsec += delayMsec;

  // TODO : timer 
}
