#include "PixelFade.hpp"
#include "Arduino.h"

PixelFade::PixelFade(Strip& strip) :
  Effect(strip)
{
  pixels = new Pixel[strip.getLength()];
  targetHue = getRandomHue();
}

float PixelFade::getRandomHue()
{
  return random(999)/1000.0f;
}

void PixelFade::updateAndRender(float smallKnobValue) 
{
    float brightness = smallKnobValue * 0.4f;

    // Seed random based on selected hue so that pixels have different
    // random speeds each time
    randomSeed(targetHue *1200);

    float maxDifference = 0.0f;

    for (int i = 0; i < strip.getLength(); i++)
    {
      Pixel& pixel = pixels[i];
      
      // Move each pixel towards the selected hue at it's own random speed
      float pixelSpeed = random(500) / 1000.0f + 0.5f;
      pixel.hue += (targetHue - pixel.hue) * pixelSpeed * 0.002f;
      strip.setPixelHSVf(i, pixel.hue, 0.4f, brightness);

      maxDifference = max(maxDifference, abs(pixel.hue - targetHue));
    }
/*
    // When all pixels are close to the target hue 
    // select a new hue at random
    if(maxDifference < 0.08f)  
    {
      targetHue = getRandomHue();
    }
    */
}
