#include "SHeartbeat.h"
#include "SRecorder.h"
#include "SIMU.h"
#include "WiFiEsp.h"
#include <ArduinoJson.h>
#include "SoftwareSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define REC_PIN 1
#define HB_PIN 2

#define DECIBEL_ALARM 1
#define HEART_ALARM 2
#define TUMBLE_ALARM 3


char ssid[] = "leegunjoo";     //  your network SSID (name) 1575835037
char pass[] = "12345670";    // your network password

IPAddress hostIp(15, 165, 113, 25);
int SERVER_PORT = 8080;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

SoftwareSerial Wifiesp(2, 3);
WiFiEspClient client;
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

SSound SM;
SHeartbeat SH;
SIMU SI;

boolean flag[3];

boolean WiFiconnect()
{
  WiFi.init(&Wifiesp);
  status = WiFi.begin(ssid, pass);
  // 현재 아두이노에 연결된 실드를 확인
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    return false;
  } else {
    Serial.println("wifi shield is load...");
  }

  // firmware version 확인
  Serial.print(F("Firmware version: "));
  Serial.println(WiFi.firmwareVersion());

  // 연결되지 않으면 여기서 종료됨.
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    return false;
  }
  // 연결되면, 연결 정보 출력 :
  else {
    Serial.println("Connected to network");
    return true;
  }
}

void JsonAlarm(int alarmType) {
  switch (alarmType) {
    case DECIBEL_ALARM:
      root["decibel"] = "high";
      break;
    case HEART_ALARM:
      root["heart"] = "high";
      break;
    case TUMBLE_ALARM:
      root["tumble"] = "fall";
      break;
  }
}

void Jsonparse(int alarmType) {
  Serial.println("jsonparsing start...");

  while (!WiFiconnect());
  client.connect(hostIp, SERVER_PORT);

  Serial.println("server Connecting...");
  //POST Data Set//
  String jsondata = "";

  JsonAlarm(alarmType);

  root.printTo(jsondata); //String으로 변환/

  // send the HTTP POST request
  //  http://ec2-15-165-113-25.ap-northeast-2.compute.amazonaws.com:8080
  //client.print(F("POST /api/v1/users "));
  client.print(F("POST "));
  client.print(F("/api/v1/mlos/mlo1/alarms "));
  client.print(F("HTTP/1.1\r\n"));
  // client.print(F("Cache-Control: no-cache\r\n"));
  client.print(F("Host: ec2-15-165-113-25.ap-northeast-2.compute.amazonaws.com:8080\r\n"));
  client.print(F("User-Agent: Arduino\r\n"));
  client.print(F("Content-Type: application/json; charset=UTF-8\r\n"));
  client.print(F("Content-Length: "));
  client.println(jsondata.length());
  client.println();
  client.println(jsondata);
  client.print(F("\r\n\r\n"));
  // note the time that the connection was made
  //lastConnectionTime = millis();
  //getIsConnected = true;
  Serial.println("transmit complete");

  client.flush();
  client.stop();
}

void setup()
{
  Serial.begin(9600);
  Wifiesp.begin(9600); //9600
  while (!WiFiconnect());
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
  SH.PlayMilli(2, 60, 80);   // 60 milli seconds

  SM.PlayMicro(1, 125000, 500);    // 125 micro second
  SM.PlayMilli(1, 1000, 500);  // 100 milli to decrease

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

      /*if (SH.Active(raw))
        {
        int bpm = SH.getBPM();
        Serial.print(bpm);
        Serial.println(" bpm");
        }*/
    }

    /** Decibel
    */
    if (SM.UpdateMicro(deltaMicro))
    {
      int raw = analogRead(REC_PIN) >> 2;
      if (SM.Sample(raw))
      {
        Serial.println("sample high");
      }
    }

    if (SM.UpdateMilli(deltaMilli)) // true if 0.1sec from last high value
    {
      //Serial.println("----high----");
      Jsonparse(DECIBEL_ALARM);
    }


    /** IMU

    */
    if (SI.UpdateMilli(deltaMilli))
    {
      // get raw value
      if (SI.Active())
      {
        //Serial.println("fall down");
        Jsonparse(TUMBLE_ALARM);
      }
    }

    oldMicro = currentMicro;
    oldMilli = currentMilli;

  }

  Serial.println("can't run program...");
  while (1)
  {

  }
}
