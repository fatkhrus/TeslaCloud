#include "TeslaCloud.h"

IPAddress ip(192,168,1,55); //setup static IP address
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password and deviceid

void setup(){
  Serial.begin(9600);

  //cloudclient.connect(); //connect by getting IP address for DHCP. This command takes more sketch memory than connecting by using static IP address
  cloudclient.connect(ip); //connect by using static IP address
  
  Tag tag("BUT", 7, INPUT_PULLUP); //setup tag with name "BUT" for reading value from 7 pin 
  cloudclient.addTag(tag); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
   Tag tag2("L3", 3, OUTPUT, DIGITAL_PIN); //setup tag with name "L3" for writing value to 3 pin
  cloudclient.addTag(tag2); // bind tag the cloud
}

void loop(){    
  if (cloudclient.run()); //every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}
