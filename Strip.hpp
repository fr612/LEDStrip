#ifndef STRIP_HPP
#define STRIP_HPP

#include <Adafruit_NeoPixel.h>

class Strip
{

public:
  Strip(Adafruit_NeoPixel& strip, uint16_t startOffset, uint16_t length);

  uint16_t getLength();
  
  void setPixelRGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  
  void setPixelRGBf(uint16_t index, float r, float g, float b);
  void setPixelHSVf(uint16_t index, float h, float s, float v);

private:
  Adafruit_NeoPixel& strip;
  uint16_t startOffset;
  uint16_t length;

  float* cubehelixHSV2RGB(float h, float s, float l, float* rgb);
};

#endif