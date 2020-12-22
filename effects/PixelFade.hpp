#ifndef PIXELFADE_HPP
#define PIXELFADE_HPP

#include "Effect.hpp"
#include "../Strip.hpp"

template <int N>
class PixelFade : public Effect
{

private:
  
  float hues[N];
  float velocities[N];

  float dragScalar;
  float targetHue;

  float getRandomFloat()
  {
    return random(10000)/10000.0f;
  };

  void explodeVelocities() 
  {
    for (int i = 0; i < N; ++i) 
    {
      float randomWithDirection = (getRandomFloat() * 2.0f) - 1.0f;
      velocities[i] = 0.15f * randomWithDirection;
    }
  }

  void reset()
  {
    explodeVelocities();
    targetHue = getRandomFloat() * 4.0f;
  }

public:
  PixelFade( float dragScalar = 1.0f) :
    dragScalar(dragScalar)
  {
    float startHue = getRandomFloat();
    memset(&hues, startHue, sizeof(float) * N);

    reset();
  };
  
  void updateAndRender(Strip& strip, float smallKnobValue)
  {
    float brightness = smallKnobValue * 0.4f;

    float maxDifference = 0.0f;

    for (int i = 0; i < N; i++)
    {
      // Update velocities
      float distance = targetHue - hues[i];

      float attractionForce = distance * 0.002f;
      velocities[i] += attractionForce;

      float damping = -velocities[i] * 0.0075f * dragScalar;
      velocities[i] += damping;
      
      // Update hue with velocity
      hues[i] += velocities[i];

      // Wrap hue between 0.0 and 1.0 for rendering
      float renderHue = hues[i] - (int)hues[i];
      if (hues[i] < 0) renderHue = 1 + renderHue;
      strip.setPixelHSVf(i, renderHue, 0.4f, brightness);

      maxDifference = max(maxDifference, abs(distance));
    }

    // When all pixels are close to the target hue 
    // select a new hue at random
    if(maxDifference < 0.005f)  
    {
      reset();
    }
  };

};

#endif