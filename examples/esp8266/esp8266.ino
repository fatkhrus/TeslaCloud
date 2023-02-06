#define DEBUG
#include "TeslaCloud.h"
#include <DHT.h>
#include "TagCloud.h"
#include "WebSettings.h"
#include <LiquidCrystal_I2C.h>
#include <LittleFS.h>

TeslaCloud cloudclient;
WebSettings websettings;
LiquidCrystal_I2C LCD(0x27,16,2); 
DHT dht(14, DHT11); 

void setup() {
  delay(1000);
  Serial.begin(115200);
if (!LittleFS.begin()) Serial.println("FS Error");
//cloudclient.setHost("192.168.1.8");
//cloudclient.setPort(7002);
cloudclient.setFS(&LittleFS);
websettings.setTeslaCloud(&cloudclient);
dht.begin(); 
LCD.init();
LCD.clear();
LCD.backlight();  // lcd backlight on
cloudclient.connect();
  websettings.init();
  
}
void dhthandler(){
  float h = dht.readHumidity();
   float t = dht.readTemperature();
  cloudclient.writeValue("Humidity",h);
  cloudclient.writeValue("Temperature",t);
   
   LCD.clear();
   LCD.setCursor(0,0);
   LCD.print(cloudclient.readValue("LCD"));

}
void loop() {
websettings.tick();
if (cloudclient.run()){
  dhthandler();
}  
}
