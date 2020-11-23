#ifndef PIXELFADE_HPP
#define PIXELFADE_HPP

#include "Effect.hpp"

class PixelFade : public Effect
{
public:
  PixelFade(Strip& strip);
  
  void updateAndRender(float smallKnobValue);

private:
  
  struct Pixel
  {
    float hue = 0.0f;
    float velocity = 0.0f;
  };

  Pixel* pixels;

  float targetHue;

  float getRandomHue();
};

#endif