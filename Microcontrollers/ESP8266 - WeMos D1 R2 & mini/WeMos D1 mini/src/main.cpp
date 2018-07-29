/*
 *wemos d1 mini is connected with light dimmer (lamp)
 */
#include "mynetwork_ESP8266.h"

#define deviceName "WeMos D1 mini"
String proccessData(String data);

int g_lamp;
void setup() {
  Serial.begin(115200);
  mynetwork_init(5551, 5551);
  g_lamp = random(0, 100) ;

}

void loop() {
  //check network TCP/UDP buffer
  //if it not empty then process data
  //and send response back of processed data is no empty.
  sendUDP(proccessData(readAllUDP())) ;

  sendTCP(proccessData(readAllTCP())) ;

}

String proccessData(String data)
{
  String header, replay ; // mac address and device name ;
  header = deviceName;
  header += ":" ;

  // if it's empty return nothing
  if(data.length() == 0) return replay ;
  /*
  // auto connect to server when it available
  if( data.indexOf("server;is;") >= 0)
  {
    data.replace("server;is;","");
    if(connectToHost(data)) sendTCP(deviceName);
    else sendUDP("ERR;"+data) ;

  }
  */
  else if( data.indexOf("lamp;set;") >= 0 )
  {
    data.replace("lamp;set;","");
    g_lamp = data.toInt();
    if(g_lamp < 0 || g_lamp > 100)
      replay = "lamp;err;"+String(g_lamp);
    else
      replay = "lamp;is;"+String(g_lamp);

  }
  else if( data.indexOf("lamp;get") >= 0 )
  {
    replay = "lamp;is;"+String(g_lamp);
  }
  else if(data.indexOf("*;get") >= 0)
  {
    replay = "lamp;is;"+String(g_lamp);
  }
  if(replay.length() == 0)
    return replay ;

  return header+replay ;

}
