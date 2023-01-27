#ifndef TeslaCloudDebug_h
#define TeslaCloudDebug_h


#ifdef DEBUG
  #define L(x) Serial.print(x)
  #define LOG(x) Serial.println(x)
  #define LOG2(x, y) Serial.println(x+y)
  #define LOG3(x, y, z) Serial.println(x+y+z)
  #define LOG4(x, y, z, a) Serial.println(x+y+z+a)
 
  #define WEBLOG(x) setDebugMessage(x);
  #define WEBLOG2(x) setDebugMessage2(x);
#else
  #define L(x) 
  #define LOG(x)
  #define LOG2(x, y)
  #define LOG3(x, y, z)
  #define LOG4(x, y, z, a)
  #define WEBLOG(x)
  #define WEBLOG2(x)
#endif





#endif