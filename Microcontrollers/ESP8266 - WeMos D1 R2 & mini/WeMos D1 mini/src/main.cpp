#include "mynetwork_ESP8266.h"

#define deviceName "WeMos D1 mini"
String proccessData(String data);



void setup() {
  Serial.begin(115200);
  mynetwork_init();

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
  String replay ;
  // if it's empty return nothing
  if(data.length() == 0) return replay ;

  if( data.indexOf("server;is;") >= 0)
  {
    data.replace("server;is;","");
    if(connectToHost(data)) sendTCP(deviceName);
    else sendUDP("ERR;"+data) ;

  }
  else if( data.indexOf("lamp;set;") >= 0 )
  {
    data.replace("lamp;set;","");
    //int valueI = data.toInt();
  }
  else if( data.indexOf("lamp;get;") >= 0 )
  {
  }
  else if( data.indexOf("wemosHeap;get;") >= 0 )
  {

  }
  else if(data == "<getAll>")
  {

  }
  return replay ;

}
