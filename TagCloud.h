#ifndef TagCloud_h
#define TagCloud_h
#include "ColorRange.h"
#include <Array.h>
#define RANGE_COUNT_MAX 10

enum View{
  VIEW_VALUE,
  VIEW_CIRPROGRESS,
  VIEW_PROGRESS,
  VIEW_ICON,
  VIEW_BUTTON,
  VIEW_INPUT,
  VIEW_SLIDER,
  VIEW_KNOB
};
enum STORAGE_PERIOD{
  DAY_1,
  DAYS_3,
  DAYS_7
};
//const char* viewStr[]={"value","cirprogress","progress","icon","button","input","slider","knob"};
struct TagCloud{
 TagCloud(){
   usevalue=true;
 }
  bool usevalue;
  String unit="";
  String description="";
  View viewtype=VIEW_VALUE;
  String icon="";
  float minvalue=0;
  float maxvalue=0;

  ColorRange color1;
  ColorRange color2;
  ColorRange bgcolor1;
  ColorRange bgcolor2;

};

#endif