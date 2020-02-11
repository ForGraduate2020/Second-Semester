#include <EtherCard.h>
 
//0은 DHCP, 1은 Static으로 설정
#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)
 
//Static IP 일 경우 설정
#if STATIC
// ethernet interface ip address
static byte myip[] = { 10,80,78,167 };
// gateway ip address
static byte gwip[] = { 10,80,78,254 };
#endif
 
// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
 
byte Ethernet::buffer[500]; // tcp/ip send and receive buffer
 
//요청이 왔을 경우 뿌려줄 HTML 소스
const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "This is Aruduino Web server"
  "</title></head>"
  "<body>"
    "<h3>Arudino</h3>"
    "<p><em>"
      "Web page<br />"
      "Under construction"
    "</em></p>"
  "</body>"
"</html>"
;
 
void setup(){
  Serial.begin(9600);
  Serial.println("\ngd");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif
 
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}
 
void loop(){
  //웹브라우저의 요청이 들어왔을 경우 page 의 내용을 보내줌
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
