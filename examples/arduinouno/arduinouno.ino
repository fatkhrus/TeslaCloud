#define DEBUG
#include "TeslaCloud.h"

byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,1,55);
TeslaCloud cloudclient("user","111111",5);

void setup(){
 delay(1000);
  Serial.begin(9600);
  Serial.println("HELLO");
  //cloudclient.setHost("192.168.1.8");
  //cloudclient.setPort(7002);
  cloudclient.connect(ip, mac);
  
  Tag tag("BUT", 7, INPUT_PULLUP);
  cloudclient.addTag(tag);
  Tag tag2("vol",A0, INPUT, ANALOG_PIN);
  tag2.setScale(0, 1023, 0, 5);
  tag2.setDeadband(0.2);
  cloudclient.addTag(tag2);
  Tag tag3("L1", 6, OUTPUT);
  cloudclient.addTag(tag3);
  Tag tag4("L2", 5, OUTPUT);
  cloudclient.addTag(tag4);
   Tag tag5("L3", 3, OUTPUT, ANALOG_PIN);
  cloudclient.addTag(tag5);
}

void loop(){    
  if (cloudclient.run()){

  };
}
