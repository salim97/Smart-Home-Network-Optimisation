#include <Arduino.h>
#include "mynetwork_W5100.h"

#define deviceName "Arduino Uno W5100"
String proccessData(String data);
int g_CO ;
int g_light_LDR ;
void setup() {
  Serial.begin(115200);
  if(mynetwork_init(5551, 5551)) Serial.println("IP:"+mynetwork_localIP());
  else Serial.println("network Failed ...") ;
  g_CO = random(0, 100) ;
  g_light_LDR = random(0, 1024);
}

void loop() {
  //traitment packet UDP if exist
  sendUDP(proccessData(readAllUDP())) ;
  //traitment packet TCP if exist
  sendTCP(proccessData(readAllTCP())) ;


}

String proccessData(String data)
{
  String replay = "";
  String header ; // mac address and device name ;
  header = deviceName;
  header += ":" ;
  
  // if it's empty return nothing
  if(data.length() == 0) return replay ;

  g_CO = random(0, 100) ;
  g_light_LDR = random(0, 1024);

  if( data.indexOf("light;get") >= 0 )
  {
    replay += "light;is;"+String(g_light_LDR);
  }
  else if( data.indexOf("CO;get") >= 0 )
  {
    replay += "CO;is;"+String(g_CO);
  }
  else if(data.indexOf("*;get") >= 0)
  {
    replay += "CO;is;"+String(g_CO);
    replay += "//";
    replay += "light;is;"+String(g_light_LDR);
  }

  if(replay.length() == 0)
    return replay ;

  return header+replay ;

}
