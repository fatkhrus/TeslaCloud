
#include "TeslaCloud.h"
#include "WebSettings.h"
#include <LittleFS.h>

TeslaCloud cloudclient; //setup cloudclient
WebSettings websettings; //setup WEB settings

void setup() {
  delay(1000);
  Serial.begin(115200);
  if (!LittleFS.begin()) Serial.println("FS Error");
  websettings.setTeslaCloud(&cloudclient); //bind cloud client and WEB settings
  cloudclient.setFS(&LittleFS);
  cloudclient.connect();  //connect cloud client
   websettings.init();  //initializi web settings
 
}
void loop() {
  websettings.tick();  //making request to web interface every cycle
  cloudclient.run();
}