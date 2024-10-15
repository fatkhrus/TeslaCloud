#define DEBUG
#include <DHT.h>
#include "TeslaCloudMQTT.h"

#include "TagCloud.h"
//#include "WebSettings.h"
//#include <LiquidCrystal_I2C.h>
#include <LittleFS.h>

//TeslaCloud cloudclient;
const char* ssid="Fatkh-2.4G"; //your router's ssid
const char* password = "13954390"; //your router's password
TeslaCloudMQTT cloudclient("myclientid","user","111111",1);

//TeslaCloudMQTT *cloudclient = new TeslaCloudMQTT("user","111111",1);
 //create cloud client with username, password of Tesla Cloud account and deviceid
//TeslaCloud cloudclient;
//WebSettings websettings;
//LiquidCrystal_I2C LCD(0x27,16,2); 
DHT dht(14, DHT11); 

void setup() {
  delay(1000);
  Serial.begin(115200);
LOG("setup");
//if (!LittleFS.begin()) Serial.println("FS Error");
//cloudclient.setHost("192.168.0.33");
//cloudclient.setPort(7002);
//cloudclient.setFS(&LittleFS);
//websettings.setTeslaCloud(&cloudclient);
dht.begin(); 
//LCD.init();
//LCD.clear();
//LCD.backlight();  // lcd backlight on
//cloudclient.connect();
 // websettings.init();
  cloudclient.connect(ssid, password); //connect to Wi-Fi by using ssid and password
 // cloudclient.subscribe("#");
  Tag tag("Temperature"); //setup tag with name "ANALOG" for reading value from 0 pin 
 
  tag.setDeadband(0.1);//setup deadband
  /*TagCloud tagcloud; //init tagcloud
  tagcloud.unit = "C"; //setup unit value
  tagcloud.description="Temperature";//description
  tagcloud.viewtype = VIEW_PROGRESS;//choose progress bar view type
  tagcloud.icon = "speed"; //choose icon 'speed'
  tagcloud.minvalue = 0; //minimum value for progress bar
  tagcloud.maxvalue = 50; //maximum value for progress bar
  tagcloud.color1 = ColorRange(0, 50, BLACK); //color for foreground
  tagcloud.bgcolor1 = ColorRange(0,30, GREEN);//colors for background
  tagcloud.bgcolor2 = ColorRange(30, 50, RED);*/
  cloudclient.addTag(tag);

  Tag tag2("Humidity"); //setup tag with name "ANALOG" for reading value from 0 pin 
  tag2.setDeadband(0.1);//setup deadband
  /*TagCloud tagcloud2; //init tagcloud
  tagcloud2.unit = "%"; //setup unit value
  tagcloud2.description="Humidity";//description
  tagcloud2.viewtype = VIEW_PROGRESS;//choose progress bar view type
  tagcloud2.icon = "speed"; //choose icon 'speed'
  tagcloud2.minvalue = 0; //minimum value for progress bar
  tagcloud2.maxvalue = 100; //maximum value for progress bar
  tagcloud2.color1 = ColorRange(0, 100, BLACK); //color for foreground
  tagcloud2.bgcolor1 = ColorRange(0,50, GREEN);//colors for background
  tagcloud2.bgcolor2 = ColorRange(50, 100, BLUE);*/
  cloudclient.addTag(tag2);
  Tag led("LED",13,OUTPUT);
  cloudclient.addTag(led);
  Tag analog("Analog", 0, INPUT, ANALOG_PIN); //setup tag with name "ANALOG" for reading value from 0 pin 
  analog.setScale(0,1023,0,5);//setup scale
  analog.setDeadband(0.1);//setup deadband
  cloudclient.addTag(analog);
  
}
void dhthandler(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  cloudclient.writeValue("Humidity",h);
  cloudclient.writeValue("Temperature",t);
   
   //LCD.clear();
   //LCD.setCursor(0,0);
   //LCD.print(cloudclient.readValue("LCD"));

}
void loop() {
//websettings.tick();
if (cloudclient.run()){
  dhthandler();
//int sensorValue = analogRead(A0);
//LOG(sensorValue);
} 
//ESP.wdtFeed(); 
}