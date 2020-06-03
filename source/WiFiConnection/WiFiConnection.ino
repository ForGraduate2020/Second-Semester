#include "WiFiEsp.h"
#include <ArduinoJson.h>

#define BAUD_RATE 9600

#ifndef HAVE_HWWifiesp
#include "SoftwareSerial.h" 
SoftwareSerial Wifiesp(2, 3); // SoftwareSerial NAME(6, 7);
#endif

char ssid[] = "leegunjoo";     //  your network SSID (name) 1575835037
char pass[] = "12345670";    // your network password


//서버의 정보= 주소,포트번호 //
IPAddress hostIp(15, 165, 113, 25);
int SERVER_PORT = 8080;
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiEspClient client;

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5000L; // delay between updates, in millisecondsunsigned long lastConnectionTime = 0;

String rcvbuf;
boolean getIsConnected = false;

// 알람 패킷 전송을 위한 변수들
String mloname = "mlo1";
String AlarmPath="/api/v1/mlos/mlo1/alarms ";
String FileUploadPath="/api/v1/mlo1/uploadFile ";
//Json객체 선언
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

// 알람을 보낼 것인가 판단하는 변수 필요 
// 알람 전송 여부를 전체적으로 판단 ex1) boolean AlarmFalg=false;
// 알람 전송 여부를 독립적으로 판단 ex2) char AlarmFalg[3] ={ 0 } //[0] : decibel, [1] : heart, [2] : tumbel
// 알람은 하나의 센서 값이라도 위험이라 판단되면 보낸다.
// 3개 중 1개라도 값이 판단되면 보내는 것은 편리
// 3개를 각각 판단할 경우 어떤 장점이 있는가?

void setup() {
  Serial.begin(BAUD_RATE); //9600
  if (!Serial){
    ; // Serial port가 연결되기를 기다린다. 
    }
  Wifiesp.begin(BAUD_RATE); //9600
  //wifi 연결 
  WiFiconnect();
}

void loop() {
  Jsonparse(false);
  Serial.println();
  delay(5000);
   //get temperature//
  Serial.println();
  delay(5000);
}
//////////////////////////
void WiFiconnect(){
  WiFi.init(&Wifiesp);
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // 현재 아두이노에 연결된 실드를 확인
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // 와이파이 실드가 아닐 경우
    while(true);
  } else{
    Serial.println("wifi shield is load...");
  }
  
  // firmware version 확인
  Serial.print(F("Firmware version: "));
  Serial.println(WiFi.firmwareVersion());

  // 연결되지 않으면 여기서 종료됨.
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // 연결되면, 연결 정보 출력 : 
  else {
    Serial.println("Connected to network");
    Serial.println("------------<network info>-----------");
    printWifiData();
    printCurrentNet();
    Serial.println("-------------------------------------");
  }
}


//////////////////////////
void JsonAlarm(){
  //decibel, heart, tumble 값을 매개변수로 받아올지, 전역변수로 받아올지 논의.
  // /api/v1/mlos/{mloName}/alarms
  root["decibel"]="decibel_nano_fuck";
  root["heart"]="heart_nano_fuck";
  root["tumble"]="tumble_nano_fuck";  
}

void JsonFileUpload(){
  // /api/v1/{mloName}/uploadFile
  root["file"]=""; //"~.wav"파일을 어떤식으로 쪼게서 보낼지 논의.
  // 1. 파일을 한 번에 변수에 넣어서 보내는 것이 베스트
  // 2. 1번의 방법이 되지 않는다면 파일을 쪼게서 보내야 한다. 
  
  root["mloName"]=""; //mloName은 #definme mloName으로 선언.
}

//////////////////////////
// /swagger-ui.html#!/alarm-api-controller/findRecordByAlarmIdUsingGET
void Jsonparse(boolean flag){ //flag=false일 경우 알람 등록, true일 경우 wav 파일 전송
  Serial.println("jsonparsing start...");
  delay(1000);
  // if there's a successful connection
  if (client.connect(hostIp, SERVER_PORT)){
    Serial.println("server Connecting...");
    //POST Data Set//
    String jsondata = "";
  
    //StaticJsonBuffer<200> jsonBuffer;
    //JsonObject& root = jsonBuffer.createObject();
    //root["username"] ="TestUser2";

    if (flag)
      JsonFileUpload();
    else
      JsonAlarm();
      
    root.printTo(jsondata); //String으로 변환/
    Serial.println(jsondata);
    
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
    lastConnectionTime = millis();
    getIsConnected = true;
    Serial.println("transmit complete");
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed!!!");
    getIsConnected = false;
  }

  client.flush();
  client.stop();
}
//////////////////////////
//////////////////////////
void printWifiData() {
  // WI-FI 실드의 IP를 출력한다.
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  
  // MAC어드레스를 출력한다.
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void printCurrentNet() {
  // 접속하려는 네트워크의 SSID를 출력한다.
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // 접속하려는 router의 MAC 주소를 출력한다.
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

 
}
