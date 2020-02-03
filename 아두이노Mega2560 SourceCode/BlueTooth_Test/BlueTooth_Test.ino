#define MEGA2560 1              //0: Uno, 1: Mega 2560

#if MEGA2560
#define mySerial Serial3
#else
#include <SoftwareSerial.h>
int blueTx = 2;
int blueRx = 3;
SoftwareSerial mySerial(blueTx, blueRx);
#endif

#define BAUDRATE 9600              //select 9600 or 115200 w.r.t module

void setup()
{
  Serial.begin(9600);
  mySerial.begin(BAUDRATE);

  pinMode(8, OUTPUT);                   //HC-05,   EN
  digitalWrite(8, HIGH);
  Serial.println("ATcommand");          //ATcommand Start
}
void loop()
{
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
