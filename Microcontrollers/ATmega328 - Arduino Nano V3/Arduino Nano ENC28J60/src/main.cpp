#include <Arduino.h>
#include "mynetwork_ENC28J60.h"

#define deviceName "Arduino Nano ENC28J60"
String proccessData(String data);
int g_servoMotor ;
int g_temperateur_TMP36 ;

void setup() {
  Serial.begin(115200);

  if(mynetwork_init(5551, 5551)) Serial.println("IP:"+mynetwork_localIP());
  else Serial.println("network Failed ...") ;
  g_servoMotor = random(0, 100) ;
  g_temperateur_TMP36 =  random(0, 40) ;
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
  String header ; // mac address and device name ;
  header = deviceName;
  header += ":" ;
  
  // if it's empty return nothing
  if(data.length() == 0) return replay ;
  g_servoMotor = random(0, 100) ;
  g_temperateur_TMP36 =  random(0, 40) ;

  if( data.indexOf("servoMotor;set;") >= 0 )
  {
    data.replace("servoMotor;set;","");
    g_servoMotor = data.toInt();
    if(g_servoMotor < 0 || g_servoMotor > 360)
      replay = "servoMotor;err;"+String(g_servoMotor);
    else
      replay = "servoMotor;is;"+String(g_servoMotor);

  }
  else if( data.indexOf("servoMotor;get") >= 0 )
  {
    replay = "servoMotor;is;"+String(g_servoMotor);
  }
  else if( data.indexOf("temperateur_TMP36;get") >= 0 )
  {
    replay = "temperateur_TMP36;is;"+String(g_temperateur_TMP36);
  }
  else if(data.indexOf("*;get") >= 0)
  {
    replay = "servoMotor;is;"+String(g_servoMotor);
    replay += "//";
    replay += "temperateur_TMP36;is;"+String(g_temperateur_TMP36);
  }
  if(replay.length() == 0)
    return replay ;

  return header+replay ;

}
