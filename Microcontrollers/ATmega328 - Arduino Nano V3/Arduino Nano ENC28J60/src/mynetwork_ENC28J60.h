#include <SPI.h>
#include <EtherCard.h>
#include <IPAddress.h>

#define enable_debug_udp false
// ethernet interface ip address
static byte myip[] = { 192,168,1,101 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };

//---------------------------------------------------------
// UDP packet handler
//---------------------------------------------------------

static uint8_t ipBroadCast[IP_LEN];
String localIP, remoteIP ;
unsigned int m_udpPort = 7456 ;
unsigned int m_tcpPort = 5544;  // port input data
String tcpBuffer, udpBuffer ;

//---------------------------------------------------------
// init ethernet
// static mac address
// dynamic ip ( DHCP client )
//---------------------------------------------------------

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[700];
void readyRead(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);

bool mynetwork_init(int udpPort, int tcpPort)
{
  m_udpPort = udpPort;
  m_tcpPort = tcpPort;
  Serial.print("Connecting to ENC28J60...");
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0)
  {
    Serial.println(F("Failed to access Ethernet controller"));
    return false ;
  }
  else
  Serial.println(" done");

  Serial.println(F("Setting up DHCP"));

  if (!ether.dhcpSetup())
  {
    Serial.println(F("DHCP failed"));
    ether.staticSetup(myip, gwip);
  }
  else
  Serial.println(" done");

  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);


  ipBroadCast[0] = ether.myip[0] ;
  ipBroadCast[1] = ether.myip[1] ;
  ipBroadCast[2] = ether.myip[2] ;
  ipBroadCast[3] = 255;
  ether.udpServerListenOnPort(&readyRead, m_udpPort); // set udp port for listen...
  localIP +=       String(ether.myip[0]) ;
  localIP += +"."+ String(ether.myip[1]) ;
  localIP += +"."+ String(ether.myip[2]) ;
  localIP += +"."+ String(ether.myip[3]) ;


  return true ;
}

String mynetwork_localIP()
{
  return  localIP;
}

void readyRead(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  //IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  // Serial.print("dest_port: ");
  // Serial.println(dest_port);
  // Serial.print("src_port: ");
  // Serial.println(src_port);
  //
  //
  // Serial.print("src_port: ");
  // ether.printIp(src_ip);
  if(enable_debug_udp)
  {
    Serial.println("data: ");
    Serial.println(data);
  }
  udpBuffer = String(data);
}
void sendUDP(String msg)
{
  if(msg.length() == 0 ) return ;

  msg += " " ;
  // convert string to char array
  int UDP_PACKET_SIZE = msg.length();
  char tmpBuffer[UDP_PACKET_SIZE-1] ;
  msg.toCharArray(tmpBuffer, UDP_PACKET_SIZE) ;

  // send msg broadcast to port destinie
  ether.sendUdp(tmpBuffer, sizeof(tmpBuffer), m_udpPort, ipBroadCast, m_udpPort );
  if(enable_debug_udp)
    Serial.println("sendUDP: "+msg) ;
Serial.println("sendUDP: "+String(msg.length())) ;
}
/*
String readAllUDP()
{
int packetSize = udp.parsePacket();
udpBuffer = "";
if(packetSize)
{
// read the packet into packetBufffer
udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
udpBuffer = String(packetBuffer) ;

for (int i =0; i < packetSize; i++)
packetBuffer[i]= 0;
Serial.println("readAllUDP: "+udpBuffer) ;
}

return udpBuffer ;
}

String readAllTCP()
{
tcpBuffer = "" ;
if (client.connected())
{
if(client.available())
{
while(client.available())
{
char c = client.read();
tcpBuffer += c ;

}

//Serial.println("readAllTCP: "+tcpBuffer) ;
}
}

return tcpBuffer ;
}

bool connectToHost(String ip)
{
if( ip == remoteIP )
if(client.connected())
return true ;

remoteIP= ip ;
byte parts[4] = {0, 0, 0, 0};
int part = 0 ;
for ( int i = 0 ; i < ip.length() ; i++)
{
char c = ip[i];
if ( c == '.' )
{
part++;
continue ;
}
parts[part] *= 10 ;
parts[part] += c - '0';
}

client.stop();
return client.connect(parts, m_tcpPort) ;
}
*/
// String ipToString(IPAddress ip)
// {
//   return String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]) ;
// }
// bool isConnected()
// {
//   return client.connected();
// }



// it should be bool and not void because TCP ....
// void sendTCP(String msg)
// {
//   if(msg.length() == 0 ) return ;
//   if(isConnected())
//   client.print(msg);
//
//   //Serial.println("sendTCP: "+msg) ;
// }
