#ifndef EEPromManager_h
#define EEPromManager_h
#include <EEPROM.h>
#include "TagSettings.h"
#include "TeslaCloud.h"

TagSettings tagsettings[TAG_COUNT_MAX];
WiFiConfig wificonfig;
TeslaCloudConfig cloudconfig;
TimeServerConfig timeserver;
DebugConfig debugconfig;
ScreenConfig screenconfig;
void saveConfigToEEPROM(){
  EEPROM.put(0, wificonfig);             
    EEPROM.commit(); 
    EEPROM.put(50,cloudconfig);
    EEPROM.commit();
    EEPROM.put(102, timeserver);
    EEPROM.commit();
    EEPROM.put(104, debugconfig);
    EEPROM.commit();
    EEPROM.put(106, screenconfig);
    EEPROM.commit();

  for (int i=0;i<TAG_COUNT_MAX;i++){
      EEPROM.put(200+i*TAG_SETTINGS_MEMORY_RESERVE,tagsettings[i]);
      EEPROM.commit();
  }
}
void checkcorrecteepromvalue(char field[]){
  for (int i=0;i<strlen(field);i++){
   if ((int)field[i]==255){
     strcpy(field,"");
     break;
   };
 }
}
void readConfigFromEEPROM(){
   LOG("read EEPROM");
  EEPROM.begin(2000);
  EEPROM.get(0, wificonfig);
  EEPROM.get(50, cloudconfig);
  checkcorrecteepromvalue(wificonfig.ssid);
  checkcorrecteepromvalue(wificonfig.pass);
  checkcorrecteepromvalue(cloudconfig.username);
  checkcorrecteepromvalue(cloudconfig.password);
 // EEPROM.get(101, devicenum);
  if (cloudconfig.devicenumber==255)
    cloudconfig.devicenumber=1;
  EEPROM.get(102, timeserver);
  if (timeserver.timezone>24 || timeserver.timezone<-24)
    timeserver.timezone = 0;
  //EEPROM.get(103, ntpserver);
  if (timeserver.ntpserver>10)
    timeserver.ntpserver=0;
  EEPROM.get(104, debugconfig);
    if (debugconfig.usedebugfile>1)
      debugconfig.usedebugfile = 0;
  //EEPROM.get(105, debugstorageperiod);
    if (debugconfig.storageperiod>10)
      debugconfig.storageperiod=0;
  EEPROM.get(106, screenconfig);
    if (screenconfig.usescreenfiles>1)
      screenconfig.usescreenfiles = 0;
  checkcorrecteepromvalue(screenconfig.mainscreen);
  int numberoftags = TAG_COUNT_MAX;
  if (numberoftags>20) numberoftags=20;
  for (int i=0;i<numberoftags;i++){
      EEPROM.get(200+i*TAG_SETTINGS_MEMORY_RESERVE,tagsettings[i]);
      checkcorrecteepromvalue(tagsettings[i].name);
      if (tagsettings[i].enable>1)
        tagsettings[i].enable=0;
      if (tagsettings[i].mode>10)
        tagsettings[i].mode=0;
      if (tagsettings[i].pin>50)
        tagsettings[i].pin=0;
      if (tagsettings[i].pintype>1)
        tagsettings[i].pintype=0;
      if (isnan(tagsettings[i].scale))
        tagsettings[i].scale=1;
      if (isnan(tagsettings[i].offset))
        tagsettings[i].offset=0;
      if (isnan(tagsettings[i].deadband))
        tagsettings[i].deadband=0;
      if (tagsettings[i].usevalue>1)
        tagsettings[i].usevalue=0;
      if (tagsettings[i].viewtype>10)
        tagsettings[i].viewtype=0;
      checkcorrecteepromvalue(tagsettings[i].description);
      checkcorrecteepromvalue(tagsettings[i].unit);
      checkcorrecteepromvalue(tagsettings[i].icon);
      if (isnan(tagsettings[i].minvalue))
        tagsettings[i].minvalue=0;
      if (isnan(tagsettings[i].maxvalue))
        tagsettings[i].maxvalue=0;
      if (isnan(tagsettings[i].color1from))
        tagsettings[i].color1from=0;
      if (isnan(tagsettings[i].color1to))
        tagsettings[i].color1to=0;
      if (tagsettings[i].color1> 0xffffff)
        tagsettings[i].color1 = 0x000000;
      if (isnan(tagsettings[i].color2from))
        tagsettings[i].color2from=0;
      if (isnan(tagsettings[i].color2to))
        tagsettings[i].color2to=0;
      if (tagsettings[i].color2> 0xffffff)
        tagsettings[i].color2 = 0x000000;

        if (isnan(tagsettings[i].bgcolor1from))
        tagsettings[i].bgcolor1from=0;
      if (isnan(tagsettings[i].bgcolor1to))
        tagsettings[i].bgcolor1to=0;
      if (tagsettings[i].bgcolor1> 0xffffff)
        tagsettings[i].bgcolor1 = 0xffffff;
      if (isnan(tagsettings[i].bgcolor2from))
        tagsettings[i].bgcolor2from=0;
      if (isnan(tagsettings[i].bgcolor2to))
        tagsettings[i].bgcolor2to=0;
      if (tagsettings[i].bgcolor2> 0xffffff)
        tagsettings[i].bgcolor2 = 0xffffff;
      if (tagsettings[i].history>1)
          tagsettings[i].history=0;
      if (tagsettings[i].storageperiod>10)
          tagsettings[i].storageperiod=0;
  }
  //LOG(sizeof(tagsettings[0]));
}
void fillConfig(TeslaCloud *teslacloud){
  LOG("fillConfig");
   teslacloud->setWiFiConfig(wificonfig);
   teslacloud->setTeslaCloudConfig(cloudconfig);
   teslacloud->setTimeServerConfig(timeserver);
   teslacloud->setDebugConfig(debugconfig);
   teslacloud->setScreenConfig(screenconfig);
   teslacloud->clearTags();
   for(int i=0;i<TAG_COUNT_MAX; i++){
          if (!tagsettings[i].enable) continue;
          Tag tag(tagsettings[i].name);
          if (tagsettings[i].mode>0){
           // tag.mode = tagsettings[i].mode-1;
           // tag.pin = tagsettings[i].pin;
           // pinMode(tag.pin, tag.mode);
            tag.setPinMode(tagsettings[i].pin, tagsettings[i].mode);           
            tag.pintype = pinType(tagsettings[i].pintype);
            if (tag.pintype>0){
              tag.k= tagsettings[i].scale;
              tag.offset = tagsettings[i].offset;
              tag.deadband = tagsettings[i].deadband;  
            }
          }
          else{
             tag.deadband = tagsettings[i].deadband;
          }
          tag.history = tagsettings[i].history;
          tag.storagePeriod = tagsettings[i].storageperiod;
          TagCloud tagcloud1;
          
          tagcloud1.usevalue = tagsettings[i].usevalue;
          tagcloud1.viewtype = View(tagsettings[i].viewtype);
          tagcloud1.description = tagsettings[i].description;
          tagcloud1.icon=tagsettings[i].icon;
          tagcloud1.minvalue = tagsettings[i].minvalue;
          tagcloud1.maxvalue = tagsettings[i].maxvalue;
          tagcloud1.color1 = ColorRange(tagsettings[i].color1from,tagsettings[i].color1to,tagsettings[i].color1);
          tagcloud1.color2 = ColorRange(tagsettings[i].color2from,tagsettings[i].color2to,tagsettings[i].color2);
          tagcloud1.bgcolor1 = ColorRange(tagsettings[i].bgcolor1from,tagsettings[i].bgcolor1to,tagsettings[i].bgcolor1);
          tagcloud1.bgcolor2 = ColorRange(tagsettings[i].bgcolor2from,tagsettings[i].bgcolor2to,tagsettings[i].bgcolor2);
          teslacloud->addTag(tag, tagcloud1);       
   }
}
void readConfig(TeslaCloud *teslacloud){
    readConfigFromEEPROM();
    fillConfig(teslacloud);
}
class EEPromManager{
  public:
  void fillTeslaCloud(TeslaCloud *cloud){
        readConfig(cloud);
      }

};

#endif