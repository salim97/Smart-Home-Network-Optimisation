#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

//---------------------------------------------------------
// UDP packet handler
//---------------------------------------------------------
IPAddress ipBroadCast;
String localIP, remoteIP ;
unsigned int udpPort = 5551 ;
unsigned int tcpPort = 5544;  // port input data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
EthernetUDP  udp; // udp broadcast client
EthernetClient client; // tcp client
String tcpBuffer, udpBuffer ;
#define DEBUG false
//---------------------------------------------------------
// init ethernet
// static mac address
// dynamic ip ( DHCP client )
//---------------------------------------------------------
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

bool mynetwork_init()
{
  Serial.println("START DHCP client...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    return false;
  }

  Serial.print("Ethernet.localIP(): ");
  Serial.println(Ethernet.localIP());
  Serial.print("Ethernet.subnetMask(): ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Ethernet.gatewayIP(): ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("Ethernet.dnsServerIP(): ");
  Serial.println(Ethernet.dnsServerIP());

  ipBroadCast = Ethernet.localIP() ;
  ipBroadCast[3] = 255;
  udp.begin(udpPort); // set udp port for listen...
  localIP +=       String(Ethernet.localIP()[0]) ;
  localIP += +"."+ String(Ethernet.localIP()[1]) ;
  localIP += +"."+ String(Ethernet.localIP()[2]) ;
  localIP += +"."+ String(Ethernet.localIP()[3]) ;


  return true ;
}

String mynetwork_localIP()
{
  return  localIP;
}

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
    if(DEBUG)
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
  for ( unsigned int i = 0 ; i < ip.length() ; i++)
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
  return client.connect(parts, tcpPort) ;
}
String ipToString(IPAddress ip)
{
    return String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]) ;
}
bool isConnected()
{
  return client.connected();
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
  udp.beginPacket(ipBroadCast, udpPort);
  udp.write(tmpBuffer, sizeof(tmpBuffer));
  udp.endPacket();
  if(DEBUG)
  Serial.println("sendUDP: "+msg) ;

}

// it should be bool and not void because TCP ....
void sendTCP(String msg)
{
  if(msg.length() == 0 ) return ;
  if(isConnected())
    client.print(msg);

  //Serial.println("sendTCP: "+msg) ;
}
