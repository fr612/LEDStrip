
#ifndef STRIP_HPP
#define STRIP_HPP

#include <Adafruit_NeoPixel.h>

class Strip
{

public:
  Strip(Adafruit_NeoPixel& strip);
  Strip(Adafruit_NeoPixel& strip, int startOffset, int length);

  int getLength();
  
  void setPixelRGB(int index, uint8_t r, uint8_t g, uint8_t b);
  
  void setPixelRGBf(int index, float r, float g, float b);
  void setPixelHSVf(int index, float h, float s, float v);

private:
  Adafruit_NeoPixel& strip;
  int startOffset;
  int length;

  float* cubehelixHSV2RGB(float h, float s, float l, float* rgb);
};

#endif