#include <Arduino.h>
#include "mynetwork_ENC28J60.h"

#define deviceName "Arduino Nano ENC28J60"
String proccessData(String data);

void setup() {
  Serial.begin(115200);

  if(mynetwork_init()) Serial.println("IP:"+mynetwork_localIP());
  else Serial.println("network Failed ...") ;


}

void loop() {
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
  //traitment packet UDP if exist
  if(udpBuffer.length() != 0)
  {
    //sendUDP( XOR( proccessData( udpBuffer ) ) ) ;
    sendUDP( proccessData( udpBuffer ) ) ;
    udpBuffer = "" ; // clear buffer
  }


}


String proccessData(String data)
{
  String replay ;
  // if it's empty return nothing
  if(data.length() == 0) return replay ;

  if(data == "<getAll>")
  {

  }
  else if( data.indexOf("fanSpeed;set;") >= 0)
  {
      data.replace("fanSpeed;set;","");
  }
  else if( data.indexOf("lamp;set;") >= 0)
  {
      data.replace("lamp;set;","");
  }
  else if( data.indexOf("server;is;") >= 0)
  {
    data.replace("server;is;","");
  }
  return replay ;

}
