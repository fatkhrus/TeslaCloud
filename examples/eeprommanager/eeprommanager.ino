#include "TeslaCloud.h"
//#include "WebSettings.h"
#include "EEPromManager.h"
#include <LittleFS.h>

TeslaCloud cloudclient; //setup cloudclient
//WebSettings websettings; //setup WEB settings
EEPromManager eeprom; //setup EEPromManager
void setup() {
  delay(1000);
  Serial.begin(115200);
  if (!LittleFS.begin()) Serial.println("FS Error");
 // websettings.setTeslaCloud(&cloudclient); //bind cloud client and WEB settings
 eeprom.fillTeslaCloud(&cloudclient);//fill Tesla cloud client's from EEPROM. Should be before connection

  cloudclient.setFS(&LittleFS);
  cloudclient.connect();  //connect cloud client
 //  websettings.init();  //initializi web settings
 
}
void loop() {
  //websettings.tick();  //making request to web interface every cycle
  cloudclient.run();
}