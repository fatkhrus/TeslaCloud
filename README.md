# TeslaCloud C++ library

Communication protocol for connecting to TeslaCloud by using TeslaSCADA2 possibilities.
### TeslaSCADA2 is a SCADA that supports many industrial and IoT protocols (OPC UA, Modbus, Siemens ISO/TCP, MQTT, Ethernet/IP, FINS/TCP and others) for connecting to hardware and visualize industrial processes.
TeslaSCADA is HMI/SCADA for Windows, MacOS, Linux, Android and iOS.
It is a set of native multi-platform applications and services for developing Human Machine Interfaces for real time monitoring of industrial PLC based systems and processes. 
TeslaSCADA products allow the control of automated processes to be extended to any device like PC, smartphone, tablet or even smartwatch. 
In last versions of TeslaSCADA it's possible to represent all datas in the cloud.
For more information about teslascada visit our site: https://teslascada.com
 
 ### TeslaCloud is a library for connecting IoT hardware to the cloud.
- With TeslaCloud Library you can connect most popular hardwares (including ESP8266, ESP32, all Arduinos)to the Tesla Cloud.

- With TeslaSCADA apps for desktop (MacOS, Linux, Windows) you can easily create graphic interfaces for any project. And use this project for connecting to cloud on any device
(Windows, Linux, MacOS, Android. iPhone version for cloud connection under working). Or by using WEB interface. 

- Hardware can connect to Tesla Cloud over the Internet using hardware connectivity available on your board (like ESP32), or with the use of Ethernet shield.

### Compatibility
esp8266, esp32, Arduino

## Content
- [Install](#install)
- [Dependencies](#dependencies)
- [Initialization](#init)
- [Usage](#usage)
- [Example for Arduino](#examplearduino)
- [Example for ESP](#exampleesp)

<a id="install"></a>
## Installation
- [Download library](https://github.com/fatkhrus/TeslaCloud/archive/refs/heads/master.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
	
<a id="dependencies"></a>
## Dependencies
TeslaCloud use some external libraries. You have to install it also:

	- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
    - [Array](https://github.com/janelia-arduino/Array)
    - [mString](https://github.com/GyverLibs/mString)
    - [TimerMs](https://github.com/GyverLibs/TimerMs)
	- [UnixTime](https://github.com/GyverLibs/UnixTime)
	- [GyverNTP](https://github.com/GyverLibs/GyverNTP)
	- [GyverPortal](https://github.com/GyverLibs/GyverPortal)
	

<a id="init"></a>
## Initialization
```cpp
TeslaCloud teslacloud; // default init
TeslaCloud teslacloud(username, userpassword, deviceid);// User name for Tesla Cloud account, password for Tesla Cloud account, ID of the device from 1 to 255
```
<a id="usage"></a>
## Usage
```cpp
void connect();// connect to Tesla Cloud. You have to setup WI-FI settings for ESP devices and Ethernet settings for Arduino devices
void connect(const char* ssid, const char *password);// connect to Tesla Cloud by using Wi-Fi. ssid of the router, password of the router
void connect(IPAddress local,const byte mac[]=NULL);// connect to Tesla Cloud and bind current device to IP address (ONLY FOR ARDUINO)
void connect(IPAddress local, IPAddress dns, IPAddress gateway, IPAddress subnet, const byte mac[]=NULL);// connect to Tesla Cloud and use the whole IP addresses (local, dns, gateway, subnet)(ONLY FOR ARDUINO)
void setTimerPeriod(int period);//set communication period in milliseconds between Tesla Cloud and device. Should be greater then 1000ms.
void addTag(Tag tag); //add tag for monitoring by cloud
void addTag(Tag tag, TagCloud tagcloud); //add tag for monitoring and card description for the cloud if you want (ONLY FOR ESP)
char* readValue(const char* tagname);//read tag's value from the cloud. Tagname name of the tag.
void writeValue(const char* tagname, int value);//write integer value to the cloud to the tag with name tagname
void writeValue(const char* tagname, float value);//write float value to the cloud to the tag with name tagname
void writeValue(const char* tagname, bool value);//write bool value to the cloud to the tag with name tagname
void writeValue(Tag tag, int value);//write integer value to the cloud to the tag 
void writeValue(Tag tag, float value);//write float value to the cloud to the tag 
void writeValue(Tag tag, bool value);//write bool value to the cloud to the tag 
void setHost(const char* host);//set host of the Tesla cloud. By default "cloud.teslascada.com"
void setPort(uint16_t port);//set port of the Tesla cloud. By default 7002
uint8_t getTagMaxCount();//get maximum nuber of tags. You can change it in Settings. By default for ESP device 20 and for Arduino 10.
bool run();//every timer period executed (should be placed in the loop). You can use period timer for your purposes because it returns TRUE every cycle.
void setTeslaCloudConfig(TeslaCloudConfig cloudconfig); //setup Tesla Cloud configuration
//struct TeslaCloudConfig{
//  char username[25]; //username in Tesla Cloud
//  char password[25]; //password in Tesla Cloud
//  uint8_t devicenumber=0; //number of the device
//};
TeslaCloudConfig getTeslaCloudConfig(); //return Tesla Cloud configuration
void setWiFiConfig(WiFiConfig wificonfig); // set WI-FI configuration for the device
WiFiConfig getWiFiConfig(); // get WI-FI configuration for the device
// struct WiFiConfig{ // WIFIConfig is a struct with ssid and password for Wi-Fi connection.
//      char ssid[25];
//      char pass[25];
//    };
void setDebugFileMessage(String msg)//set debug message into the debug file (ONLY FOR ESP)
void setFS(fs::FS *useFS) //set file system LittleFS or SNIFFS (ONLY FOR ESP)
fs::FS * getFS();//get file system
void setTimeServerConfig(TimeServerConfig config);//set time server config (ONLY FOR ESP)
//struct TimeServerConfig{
//      int8_t timezone;   //time zone of NTP server
//      uint8_t ntpserver;  //server: 0:"pool.ntp.org", 1:"ntp1.stratum2.ru", 2: "ntp2.stratum2.ru", 3: "ntp.msk-ix.ru"
//    };
void setDebugConfig(DebugConfig config);//set debug configuration (ONLY FOR ESP)
//struct DebugConfig{  
//      uint8_t usedebugfile; // use or not debug file
//      uint8_t storageperiod;// storage period: 0:1 day, 1:3 days, 2:7 days
//    };
void setScreenConfig(ScreenConfig config);//set screen configuration (ONLY FOR ESP)
//struct ScreenConfig{
//      uint8_t usescreenfiles;  //use or not screens for cloud in this device
//      char mainscreen[20];	//main screen name. Screen uploaded by cloud when user firts open Screen tab
//    };
```
### Requirements
- You need to call `run()` in the main loop of the program `loop()`, it synchronizes communication with Tesla Clout according to its timer.

<a id="examplearduino"></a>
## Example for Arduino
```cpp
// example read pins from the Arduino UNO and send it to the Tesla Cloud
// get other tag's value and write it to Arduino other pin.

#include "TeslaCloud.h"

IPAddress ip(192,168,1,55); //setup static IP address
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password and deviceid

void setup(){
  Serial.begin(9600);
 
  //cloudclient.connect(); //connect by getting IP address for DHCP. This command takes more sketch memory than connecting by using static IP address
  cloudclient.connect(ip, mac); //connect by using static IP address
  
  Tag tag("BUT", 7, INPUT_PULLUP); //setup tag with name "BUT" for reading value from 7 pin 
  cloudclient.addTag(tag); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
   Tag tag2("L3", 3, OUTPUT, ANALOG_PIN); //setup tag with name "L3" for writing value to 3 pin
  cloudclient.addTag(tag2); // bind tag the cloud
}

void loop(){    
  if (cloudclient.run()); //every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}
```
<a id="exampleesp"></a>
## Example for ESP
```cpp
// example read pins from the ESP and send it to the Tesla Cloud
// get other tag's value and write it to ESP other pin.

#include "TeslaCloud.h"

const char* ssid="ssid"; //your router's ssid
const char* password = "password"; //your router's password
TeslaCloud cloudclient("username","password",1); //create cloud client with username, password of Tesla Cloud account and deviceid

void setup(){
  Serial.begin(115200);
 
  
  cloudclient.connect(ssid, password); //connect to Wi-Fi by using ssid and password
  
  Tag tag("BUT", 7, INPUT_PULLUP); //setup tag with name "BUT" for reading value from 7 pin 
  cloudclient.addTag(tag); // bind tag to the cloud. In the loop every timer period (default 1000ms) pin be readed and send to the cloud if the value is changed
  
   Tag tag2("L3", 3, OUTPUT, ANALOG_PIN); //setup tag with name "L3" for writing value to 3 pin
  cloudclient.addTag(tag2); // bind tag the cloud
}

void loop(){    
  if (cloudclient.run());//every cycle request pins and if the value is changed send to the Tesla Cloud. Also if new value comes from the Cloud it will be written into the output tag.
}
```