#include "TeslaCloud.h"

const char* ssid="ssid"; //your router's ssid
const char* password = "pass"; //your router's password
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password of Tesla Cloud account and deviceid

void setup(){
  Serial.begin(115200);
 

  cloudclient.connect(ssid, password); //connect to Wi-Fi by using ssid and password
  Tag tag("ANALOG", 0, INPUT, ANALOG_PIN); //setup tag with name "ANALOG" for reading value from 0 pin 
  tag.setScale(0,1023,0,5);//setup scale
  tag.setDeadband(0.1);//setup deadband
  TagCloud tagcloud; //init tagcloud
  tagcloud.unit = "V"; //setup unit value
  tagcloud.description="Voltage";//description
  tagcloud.viewtype = VIEW_PROGRESS;//choose progress bar view type
  tagcloud.icon = "speed"; //choose icon 'speed'
  tagcloud.minvalue = 0; //minimum value for progress bar
  tagcloud.maxvalue = 5; //maximum value for progress bar
  tagcloud.color1 = ColorRange(0, 5, WHITE); //color for foreground
  tagcloud.bgcolor1 = ColorRange(0,2.5, GREEN);//colors for background
  tagcloud.bgcolor2 = ColorRange(2.5, 5, RED);
  cloudclient.addTag(tag, tagcloud); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
}

void loop(){    
  if (cloudclient.run());//every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}