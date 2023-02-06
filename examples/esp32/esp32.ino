#define DEBUG
#include "TeslaCloud.h"
#include "WebSettings.h"
#include <LittleFS.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
TeslaCloud cloudclient;
WebSettings websettings;
void setup() {
  delay(1000);
  Serial.begin(115200);
  if (!LittleFS.begin()) Serial.println("FS Error");
  //cloudclient.setHost("192.168.1.8");
  //cloudclient.setPort(7002);
  cloudclient.setFS(&LittleFS);
  websettings.setTeslaCloud(&cloudclient);
  cloudclient.connect();
 websettings.init();
}
void loop() {
   websettings.tick();
  if (cloudclient.run()){
      cloudclient.writeValue("HallSensor", hallRead());
      float temp2 = (temprature_sens_read() - 32) / 1.8;
      cloudclient.writeValue("Temperature2",temp2);
    }
}
