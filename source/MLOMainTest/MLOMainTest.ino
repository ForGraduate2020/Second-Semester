#include "SHeartbeat.h"
#include "SRecorder.h"

/*#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #endif
  // Defines for setting register bits
  #ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
  #endif*/

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// set prescale to 16
/*sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;*/

#define REC_PIN 1
#define HB_PIN 2

SSound SREC;
SHeartbeat SHB;

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
  SHB.PlayMilli(2, 60, 80);   // 60 milli seconds

  SREC.PlayMicro(1, 125, 500);    // 125 micro second
  SREC.PlayMilli(1, 600000, 80);  // 10 min

  unsigned long currentMilli = 0;
  unsigned long oldMilli = millis();

  unsigned long currentMicro = 0;
  unsigned long oldMicro = micros();

  Serial.println("Start Programm...");

  // make file first
  if (!SREC.BeginRecord())
  {
    Serial.println("can't run program...");
    while(1)
    {
      
    }
  }

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
    if (SHB.UpdateMilli(deltaMilli))
    {
      int SHBraw = analogRead(HB_PIN);
      Serial.println(SHB.getMilli());
      if (SHB.Active(SHBraw))
      {
        int bpm = SHB.getBPM();
        Serial.print(bpm);
        Serial.println(" bpm");
      }
    }

    /** Record & Decibel

    */
    if (SREC.UpdateMicro(deltaMicro))
    {
      int SRECraw = analogRead(REC_PIN);

      /** Decibel

      */
      if (SREC.Sample(SRECraw))
      {
        Serial.println("-----high-----");
      }

      /** Record

      */
      SREC.Record(SRECraw);
    }

    /** Make file
        finalize current file and start new file
    */
    if (SREC.UpdateMilli(deltaMilli))
    {
      SREC.EndRecord();
      if (!SREC.BeginRecord())
      {
        break;
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

void loop() {
  // put your main code here, to run repeatedly:

}
