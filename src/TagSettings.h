#ifndef TagSettings_h
#define TagSettings_h

struct TagSettings{
  uint8_t enable;
  //tag
  char name[20];
  uint8_t mode;
  uint8_t pin;
  uint8_t pintype;
  float scale;
  float offset;
  float deadband;
  //tagcloud
  uint8_t usevalue;
  uint8_t viewtype;
  char unit[4];
  char description[20];
  char icon[10];
  float minvalue;
  float maxvalue;
  float color1from;
  float color1to;
  uint32_t color1;
  float color2from;
  float color2to;
  uint32_t color2;
  float bgcolor1from;
  float bgcolor1to;
  uint32_t bgcolor1;
  float bgcolor2from;
  float bgcolor2to;
  uint32_t bgcolor2;
  uint8_t history;
  uint8_t storageperiod;
};
#endif