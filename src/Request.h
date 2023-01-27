#ifndef Request_h
#define Request_h
#include "Tag.h"
static int ids;
class Request{
 
  public:
    Request(){

    }
    Request(int route){
      id=++ids;
      if (ids>999)
        ids=0;
      this->route = route;
    }
 
  protected:
   
    
  public:
    int id;
    int route;
    uint8_t tagid;
    #if defined(ESP8266) || defined(ESP32)
     int8_t tagcloudid;
    #endif
   // char body[BODY_SIZE];
    long requesttime=0;
    //String tagname;
};

#endif
