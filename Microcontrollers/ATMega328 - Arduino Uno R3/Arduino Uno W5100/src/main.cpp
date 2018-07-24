#include <Arduino.h>
#include "mynetwork_W5100.h"

#define deviceName "Arduino Uno W5100"
String proccessData(String data);

void setup() {
  Serial.begin(115200);
  if(mynetwork_init()) Serial.println("IP:"+mynetwork_localIP());
  else Serial.println("network Failed ...") ;

}

void loop() {
  //traitment packet UDP if exist
  sendUDP(proccessData(readAllUDP())) ;
  //traitment packet TCP if exist
  sendTCP(proccessData(readAllTCP())) ;


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
