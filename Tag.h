#ifndef Tag_h
#define Tag_h
#include <mString.h>

enum pinType{
  DIGITAL_PIN,
  ANALOG_PIN
};
class Tag{
 
  public:
  Tag(){
    this->update = false;
    this->tagvalue="0";
  }
  Tag(const char* name){
    this->name = name;
    this->tagvalue="0";
    this->update = false;
    
  }
  Tag(const char* name, uint8_t pin, uint8_t mode, pinType pintype=DIGITAL_PIN){
    this->name = name;
    this->pin = pin;
    this->mode = mode;
    this->pintype=pintype;
    this->update = false;
    pinMode(pin, mode);
 
  }
  float readFromDevice(){
 #ifdef ESP32
    if (mode==INPUT || mode==INPUT_PULLUP || mode==INPUT_PULLDOWN){
 #else
    if (mode==INPUT || mode==INPUT_PULLUP ){
 #endif
       if (this->pintype==DIGITAL_PIN)
        return mode==INPUT_PULLUP?!digitalRead(this->pin):digitalRead(this->pin);  
      else 
        return k*analogRead(this->pin)+offset;
    }
    else return 0;
  }
  void write(mString<TAGVALUE_SIZE> value){
    if (mode==OUTPUT)
      writeToDevice(value);
    this->tagvalue = value;
  }
  void writeToDevice(mString<TAGVALUE_SIZE> value){
    if (this->pintype==DIGITAL_PIN)
        digitalWrite(this->pin, value.toInt());
    else
        analogWrite(this->pin, value.toInt());
    
  }
  void run(){
    #ifdef ESP32
        if (mode==INPUT || mode==INPUT_PULLUP || mode==INPUT_PULLDOWN)
    #else
        if (mode==INPUT || mode==INPUT_PULLUP )
    #endif
       read();
  }
  void readValue(int value){
    if (pintype==ANALOG_PIN)
       setValue(k*value+offset);
    else
      setValue(value);
  }
  void readValue(float value){
     if (pintype==ANALOG_PIN)
       setValue(k*value+offset);
    else
      setValue(value);
  }
  void readValue(bool value){
    if (value)
      setValue(1);
    else
      setValue(0);
  }
  void setPinMode(uint8_t pin, uint8_t m){
    
        switch(m){
          case 0: mode = 10;
            break;
          case 1: mode = INPUT;
            break;
          case 2: mode = OUTPUT;
            break;
          case 3: mode = INPUT_PULLUP;
            break;
      #ifdef ESP32
          case 4: mode = INPUT_PULLDOWN;
            break;
      #else
           case 4: mode = 10;
            break;
      #endif
          default: mode = 10;
            break;
        }         
    this->pin = pin;
    if (mode!=10)
      pinMode(pin,mode);
  }
  void setScale(int rawmin, int rawmax, float eumin, float eumax, float offset=0){
    if (rawmax<rawmin) return;
    
    k = (eumax-eumin)/(rawmax-rawmin)+eumin;
    this->offset = offset;
  }
  void setDeadband(float deadband){
    this->deadband = deadband;
  }
  bool update;
  mString<TAGNAME_SIZE> name;
  mString<TAGVALUE_SIZE> tagvalue;
  float oldvalue;
  uint8_t mode;
  uint8_t pin;
  pinType pintype;
  float k=1;
  float offset;
  float deadband=0;
  #ifdef ESP8266 
  uint8_t history;
  uint8_t storagePeriod;
  #elif ESP32
  uint8_t history;
  uint8_t storagePeriod;
  #endif  
  void read(){
      setValue(readFromDevice());    
  }

private:
  

  void setValue(float value){
  
      if (value==oldvalue) return;
   
      if (value>oldvalue && (value-oldvalue)<deadband) return;
      if (value<oldvalue && (oldvalue-value)<deadband) return; 
      oldvalue = value;
      tagvalue =value;
      update = true;

  }

};
#endif 