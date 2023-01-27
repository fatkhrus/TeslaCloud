#ifndef Settings_h
#define Settings_h

#define SP_AP_NAME "ESP WEB Config"     // AP point name
#define SP_AP_IP 192,168,1,1    

#define RESPONSE_TIME 180000
#define REQUEST_TIME_REPEAT 5000
#define SCREENFILE_SIZE 512

#if defined(ESP8266) || defined(ESP32)
#define TAG_COUNT_MAX 20
#define REQUEST_COUNT_MAX 20

#define REQUEST_SIZE 300
#define SUBSCRIBE_SIZE 600
#define RESPONSE_SIZE 200
#define TAGNAME_SIZE 20
#define TAGVALUE_SIZE 20
#else
#define TAG_COUNT_MAX 10
#define REQUEST_COUNT_MAX 10

#define REQUEST_SIZE 80
#define RESPONSE_SIZE 80
#define TAGNAME_SIZE 10
#define TAGVALUE_SIZE 10
#endif

#define TAG_SETTINGS_MEMORY_RESERVE 150

#endif
