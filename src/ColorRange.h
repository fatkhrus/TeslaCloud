#ifndef ColorRange_h
#define ColorRange_h

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define BLUE 0x0000FF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define YELLOW 0xFFFF00
#define CYAN 0x00FFFF
#define MAGENTA 0xFF00FF

struct ColorRange{
    ColorRange(){

    }
    ColorRange(float from, float to, uint32_t color){
      this->from =from;
      this->to = to;
      this->color= color;
    }
    float from;
    float to;
    uint32_t color=0xFFFFFFFF;
};

#endif