#include "SHeartbeat.h"
#include "SRecorder.h"
#include "SIMU.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define REC_PIN 1
#define HB_PIN 2

SSound SM;
SHeartbeat SH;
SIMU SI;

void setup()
{
  Serial.begin(9600);

  // disable digital input register
  sbi(DIDR0, ADC0D);
  sbi(DIDR0, ADC1D);
  sbi(DIDR0, ADC2D);
  sbi(DIDR0, ADC3D);
  sbi(DIDR0, ADC4D);
  sbi(DIDR0, ADC5D);

  // prescalar 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;

  // initialize sensors
  SH.PlayMilli(2, 60000, 80);   // 60 milli seconds

  SM.PlayMicro(1, 125000, 500);    // 125 micro second

  SI.Initialize();
  SI.PlayMilli(0, 100, 80);

  unsigned long currentMilli = 0;
  unsigned long oldMilli = millis();

  unsigned long currentMicro = 0;
  unsigned long oldMicro = micros();

  Serial.println("Start Programm...");

  /** Main Loop

  */
  while (1)
  {
    currentMilli = millis();
    currentMicro = micros();

    unsigned long deltaMicro = currentMicro - oldMicro;
    unsigned long deltaMilli = currentMilli - oldMilli;

    /** Heartbeat

    */
    if (SH.UpdateMilli(deltaMilli))
    {
      int raw = analogRead(HB_PIN);
      
      if (SH.Active(raw))
      {
        int bpm = SH.getBPM();
        Serial.print(bpm);
        Serial.println(" bpm");
      }
    }

    /** Decibel

    */
    if (SM.UpdateMicro(deltaMicro))
    {
      int raw = analogRead(REC_PIN);

      if (SM.Sample(raw))
      {
        Serial.println("-----high-----");
      }
    }

    /** IMU
     *  
     */
    if (SI.UpdateMilli(deltaMilli))
    {
      // get raw value
      if (SI.Active())
      {
        Serial.println("fall down");
        
        /*float ax, ay, az;
        float gx, gy;
        float acc, gyro;

        SI.GetAccel(&ax, &ay, &az);
        SI.GetGyro(&gx, &gy);


        acc = sqrt(ax*ax + ay*ay + az*az);
        gyro = sqrt(gx*gx + gy*gy);
        //acc = SI.GetAccel();
        //gyro = SI.GetGyro();

        if (acc > 2.0 && gyro > 200)
        {
          Serial.println("fall");
        }*/
      }
    }

    oldMicro = currentMicro;
    oldMilli = currentMilli;
  }

  Serial.println("can't run program...");
  while(1)
  {
    
  }
}
