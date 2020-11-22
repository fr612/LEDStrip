#include "Strip.hpp"

Strip::Strip(Adafruit_NeoPixel& strip) : 
  strip(strip),
  startOffset(0),
  length(strip.getPixels())
{
}

Strip::Strip(Adafruit_NeoPixel& strip, int startOffset, int length) : 
  strip(strip),
  startOffset(startOffset),
  length(length)
{
}

int Strip::getLength()
{
    return length;
}

void Strip::setPixelRGB(int index, uint8_t r, uint8_t g, uint8_t b)
{
  int trueIndex = (index % length) + startOffset;
  strip.setPixelColor(trueIndex, r, g, b);
}
  
void Strip::setPixelRGBf(int index, float r, float g, float b)
{
    setPixelRGB(index, r*256, g*256, b*256);
}

void Strip::setPixelHSVf(int index, float h, float s, float v)
{
  float rgb[3];
  cubehelixHSV2RGB(h, s, v, rgb);
  setPixelRGBf(index, rgb[0], rgb[1], rgb[2]);
}

// Discovered from this tweet thread https://twitter.com/koujaku/status/1326361797100662785
// Modified from this gist: https://gist.github.com/maluoi/1c4e1ebd34c4837313dff32992a10183

// Reference here: http://www.mrao.cam.ac.uk/~dag/CUBEHELIX/
float* Strip::cubehelixHSV2RGB(float h, float s, float l, float* rgb) {
  const float tau = 6.28318f;
  l = min(1,l);
  float angle = tau * (h+(1/3.f));
  float amp   = s * l * (1.f - l);

  float a_cos = cos(angle);
  float a_sin = sin(angle);
  float r = l + amp * (-0.14861f * a_cos + 1.78277f * a_sin);
  float g = l + amp * (-0.29227f * a_cos - 0.90649f * a_sin);
  float b = l + amp * ( 1.97294f * a_cos);
  rgb[0] = max(0,min(1, r));
  rgb[1] = max(0,min(1, g));
  rgb[2] = max(0,min(1, b));
  return rgb;
}