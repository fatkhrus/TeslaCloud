// example read analog pin from the ESP and send it to the Tesla Cloud

#include "TeslaCloud.h"

const char* ssid="ssid"; //your router's ssid
const char* password = "password"; //your router's password
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password of Tesla Cloud account and deviceid

void setup(){
  Serial.begin(115200);
 
  
  cloudclient.connect(ssid, password); //connect to Wi-Fi by using ssid and password
  Tag tag("ANALOG", 0, INPUT, ANALOG_PIN); //setup tag with name "ANALOG" for reading value from 0 pin 
  tag.setScale(0,1023,0,5);//setup scale
  tag.setDeadband(0.1);//setup deadband
  cloudclient.addTag(tag); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
}

void loop(){    
  if (cloudclient.run());//every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}