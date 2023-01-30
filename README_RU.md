# Библиотека TeslaCloud C++

Коммуникационный протокол для подключения к TeslaCloud с использованием возможностей TeslaSCADA2.

### TeslaSCADA2 — это SCADA, поддерживающая множество промышленных и IoT-протоколов (OPC UA, Modbus, Siemens ISO/TCP, MQTT, Ethernet/IP, FINS/TCP и другие) для подключения к оборудованию и визуализации промышленных процессов.
TeslaSCADA2 это HMI/SCADA для Windows, MacOS, Linux, Android и iOS.
Это пакет нативных мульти-платформенных приложений и сервисов для разработки человеко-машинных интерфейсов для контроля в режиме реального времени индустриальных систем и процессов на базе ПЛК. 
Продукты TeslaSCADA позволяют осуществлять визуальный контроль за производством при помощи любых устройств, таких как персональный компьютер, смартфон, планшетный ПК или даже Умные часы.
В последних версиях TeslaSCADA есть возможность все отобразить в облаке.
 
### TeslaCloud это библиотека для взаимодействия IoT устройств с облаком.
- При помощи Библиотеки TeslaCloud вы можете подлючить наиболее популярные устройства (включая ESP8266, ESP32, все Arduino)к Tesla Облаку.

- При помощи TeslaSCADA приложений для ПК (MacOS, Linux, Windows) вы можете создать графический интерфейс для любого вашего проекта. И использовать данный проект на любом устройстве
(Windows, Linux, MacOS, Android. iPhone версия для взаимодействия с облаком в процессе разработки). Или использовать WEB интерфейс. 

- Устройства могут подсоединяться к облаку Tesla Cloud через Internet используя внутренние возможности платы (таких как ESP32), или при помощи Ethernet плат (таких как W5500 или W5100).

###Совместимость
esp8266, esp32, Arduino

## Содержание
- [Установка](#install)
- [Зависимости](#dependencies)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример для Arduino](#examplearduino)
- [Пример для ESP](#exampleesp)

<a id="install"></a>
## Установка
- [Скачайти библиотеку](https://github.com/fatkhrus/TeslaCloud/archive/refs/heads/master.zip) .zip архив для ручной установки:
    - Распакуйте и разместите в папке *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распакуйте и разместите в папке *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распакуйте и разместите в папке *Documents/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: Скетч/Подключить библиотеку/Добавить .ZIP библиотеку… и указать скачанный архив
	
<a id="dependencies"></a>
## Зависимости
TeslaCloud использует некоторые внешние библиотеки. Вам их необходимо также установить: [ArduinoJson](https://github.com/bblanchon/ArduinoJson), [Array](https://github.com/janelia-arduino/Array),
[mString](https://github.com/GyverLibs/mString), [TimerMs](https://github.com/GyverLibs/TimerMs), [UnixTime](https://github.com/GyverLibs/UnixTime), [GyverNTP](https://github.com/GyverLibs/GyverNTP), [GyverPortal](https://github.com/GyverLibs/GyverPortal).
	

<a id="init"></a>
## Инициализация
```cpp
TeslaCloud teslacloud; // инициализаци по умолчанию
TeslaCloud teslacloud(username, userpassword, deviceid);// Имя пользователя для аккаунта Tesla Cloud, пароль для аккаунта Tesla Cloud, ID устройства от 1 до 255
```
<a id="usage"></a>
## Использование
```cpp
void connect();// установить соединение с Tesla Cloud. Вам необходимо настроить WI-FI сначало для ESP устройств и настройки Ethernet для устройств Arduino
void connect(const char* ssid, const char *password);// установить соединение с Tesla Cloud используя Wi-Fi. Необходимо указать ssid роутера, пароль роутера (ТОЛЬКО ДЛЯ ESP)
void connect(IPAddress local,const byte mac[]=NULL);// установить соединение с Tesla Cloud и привязать текущее устройство к IP адресу (ТОЛЬКО ДЛЯ ARDUINO)
void connect(IPAddress local, IPAddress dns, IPAddress gateway, IPAddress subnet, const byte mac[]=NULL);// установить соединение с Tesla Cloud и использовать IP адреса (local, dns, gateway, subnet)(ТОЛЬКО ДЛЯ ARDUINO)
void setTimerPeriod(int period);//установить интервал взаимодействия Tesla Cloud с устройством. Должен быть больше 1000 мс
void addTag(Tag tag); //добавить тег для мониторинга в облаке
void addTag(Tag tag, TagCloud tagcloud); //добавить тег для мониторинга и описание карточки если она необходима(ТОЛЬКО ДЛЯ ESP)
char* readValue(const char* tagname);//прочитать значение тега с облака. Указать имя тега.
void writeValue(const char* tagname, int value);//записать int значение в облако указав имя тега и значение для записи
void writeValue(const char* tagname, float value);//записать float значение в облако указав име тега и значение для записи
void writeValue(const char* tagname, bool value);//записать bool значение в облако указав име тега и значение для записи
void writeValue(Tag tag, int value);///записать int значение в облако указав тег и значение для записи
void writeValue(Tag tag, float value);//записать float значение в облако указав тег и значение для записи
void writeValue(Tag tag, bool value);//записать bool значение в облако указав тег и значение для записи 
void setHost(const char* host);//указать адрес Tesla cloud. По умолчанию "cloud.teslascada.com"
void setPort(uint16_t port);//указать порт Tesla cloud. По умолчанию 7002
uint8_t getTagMaxCount();//получить максимальное значение тегов. Вы можете изменить их в Settings. По умолчанию ESP устройства имеют 20 тегов, а Arduino 10
bool run();//исполняется каждый период установленный таймером (должен быть помещен в loop). Вы можете использовать данную функция в своих целях так как она возвращает TRUE каждый цикл.
void setTeslaCloudConfig(TeslaCloudConfig cloudconfig); //настроить конфигурацию Tesla Cloud
//struct TeslaCloudConfig{
//  char username[25]; //имя пользователя в Tesla Cloud
//  char password[25]; //пароль в Tesla Cloud
//  uint8_t devicenumber=0; //номер устройства
//};
TeslaCloudConfig getTeslaCloudConfig(); //вернуть конфигурацию Tesla Cloud
void setWiFiConfig(WiFiConfig wificonfig); // настроить WI-FI конфигурацию в устройстве
WiFiConfig getWiFiConfig(); //вернуть WI-FI конфигурацию для устройства
// struct WiFiConfig{ // WIFIConfig это структура с ssid и паролем для организации Wi-Fi соединения. (ТОЛЬКО ДЛЯ ESP)
//      char ssid[25];
//      char pass[25];
//    };
void setDebugFileMessage(String msg)//записать сообщение в файл отладки (ТОЛЬКО ДЛЯ ESP)
void setFS(fs::FS *useFS) //указать файловую систему LittleFS или SPIFFS (ТОЛЬКО ДЛЯ ESP)
fs::FS * getFS();//получить файловую систему
void setTimeServerConfig(TimeServerConfig config);//установить конфигурацию сервера времени NTP (ONLY FOR ESP)
//struct TimeServerConfig{
//      int8_t timezone;   //временная зона NTP сервера
//      uint8_t ntpserver;  //NTP сервер: 0:"pool.ntp.org", 1:"ntp1.stratum2.ru", 2: "ntp2.stratum2.ru", 3: "ntp.msk-ix.ru"
//    };
void setDebugConfig(DebugConfig config);//настроить конфигурацию debug файла (ТОЛЬКО ДЛЯ ESP)
//struct DebugConfig{  
//      uint8_t usedebugfile; //использовать или нет debug файл
//      uint8_t storageperiod;//период хранения: 0:1 день, 1:3 дня, 2:7 дней
//    };
void setScreenConfig(ScreenConfig config);//настроить конфигурацию экранов (ТОЛЬКО ДЛЯ ESP)
//struct ScreenConfig{
//      uint8_t usescreenfiles;  //использовать или нет экраны для этого устройства
//      char mainscreen[20];	//имя главного экрана. Экран загружается облаком когда пользователь впервые нажимает вкладку экран
//    };
```
### Требования
- Необходимо использовать функцию `run()` в основном цикле программы `loop()`, для синхронизации обмена устройства с Tesla Cloud согласно внутреннему таймеру.

<a id="examplearduino"></a>
## Пример для Arduino
```cpp
// пример считывает данные с контакта Arduino UNO и передает значение в Tesla Cloud.
// Также в примере другой тег берет значение из облака и передает его контакту Arduino.

#include "TeslaCloud.h"

IPAddress ip(192,168,1,55); //настройки статического IP адреса
TeslaCloud cloudclient("username","password",1); //создание клиента облака с именем пользователя, паролем и ID устройства

void setup(){
  Serial.begin(9600);
 
  //cloudclient.connect(); //соединение при помощи получения IP адреса из DHCP. Это команда требует большей памяти скетча чем задание статического IP адреса
  cloudclient.connect(ip); //соединение с использование статического IP адреса
  
  Tag tag("BUT", 7, INPUT_PULLUP); //создаем тег с именем "BUT" для чтения с 7 контакта 
  cloudclient.addTag(tag); // привязываем тег к клиенту. В цикле каждый период времени (по умолчанию 1000ms) значение контакта считывается и передается в облако
  
   Tag tag2("L3", 3, OUTPUT, DIGITAL_PIN); //создаем тег с именен "L3" для записи его значения из облака в 3 контакт
  cloudclient.addTag(tag2); //привязка тега к облаку
}

void loop(){    
  if (cloudclient.run()); //в цикле каждый период времени (по умолчанию 1000ms) значение контакта считывается и передается в облако и наоборот
}
```
<a id="exampleesp"></a>
## Example for ESP
```cpp
// пример считывает данные из ESP и передает значение в Tesla Cloud
// Также в примере другой тег берет значение из облака и передает его контакту ESP.

#include "TeslaCloud.h"

const char* ssid="ssid"; //ssid роутера
const char* password = "password"; //пароль роутера
TeslaCloud cloudclient("username","password",1); //создание клиента облака с именем пользователя, паролем и ID устройства

void setup(){
  Serial.begin(115200);
 
  
  cloudclient.connect(ssid, password); //подсоединение к Wi-Fi с использованием ssid и пароля
  
  Tag tag("BUT", 7, INPUT_PULLUP); //создаем тег с имененм "BUT" для чтения значения с 7 контакта 
  cloudclient.addTag(tag); // привязываем тег к клиенту. В цикле каждый период времени (по умолчанию 1000ms) значение контакта считывается и передается в облако
  
   Tag tag2("L3", 3, OUTPUT, DIGITAL_PIN); //создаем тег с именен "L3" для записи его значения из облака в 3 контакт
  cloudclient.addTag(tag2); //привязка тега к облаку
}

void loop(){    
  if (cloudclient.run());//в цикле каждый период времени (по умолчанию 1000ms) значение контакта считывается и передается в облако и наоборот
}
```