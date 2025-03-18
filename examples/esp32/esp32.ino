#define DEBUG
#include "TeslaCloud.h"
#include "TeslaCloudSparkplugB.h"
//#include "WebSettings.h"
//#include <LittleFS.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
const char* ssid="Fatkh-2.4G"; //your router's ssid
const char* password = "13954390"; //your router's password
//TeslaCloud cloudclient("user","111111",1);
TeslaCloudSparkplugB cloudclient("user", "111111", 1,"192.168.0.33", 1883);
//WebSettings websettings;
void setup() {
  delay(1000);
  Serial.begin(115200);
 // if (!LittleFS.begin()) Serial.println("FS Error");
  //cloudclient.setHost("192.168.0.33");
  //cloudclient.setPort(7002);
 // cloudclient.setFS(&LittleFS);
  //websettings.setTeslaCloud(&cloudclient);
  cloudclient.connect(ssid, password);
  Tag tag("Temperature2",0);
  cloudclient.addTag(tag);
 //websettings.init();
}
void loop() {
   //websettings.tick();
  if (cloudclient.run()){
      //cloudclient.writeValue("HallSensor", hallRead());
      float temp2 = (temprature_sens_read() - 32) / 1.8;
      cloudclient.writeValue("Temperature2",temp2);
    }
}
