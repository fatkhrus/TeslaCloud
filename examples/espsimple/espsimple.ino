// example read pins from the ESP and send it to the Tesla Cloud
// get other tag's value and write it to ESP other pin.
#include "TeslaCloud.h"

const char* ssid="ssid"; //your router's ssid
const char* password = "password"; //your router's password
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password of Tesla Cloud account and deviceid

void setup(){
  Serial.begin(115200);
 
  cloudclient.connect(ssid, password); //connect to Wi-Fi by using ssid and password
  
  Tag tag("BUTTON", 12, INPUT_PULLUP); //setup tag with name "BUT" for reading value from 12 pin 
  cloudclient.addTag(tag); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
   Tag tag2("LED", 13, OUTPUT, DIGITAL_PIN); //setup tag with name "L3" for writing value to 13 pin
  cloudclient.addTag(tag2); // bind tag the cloud
}

void loop(){    
  if (cloudclient.run());//every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}
