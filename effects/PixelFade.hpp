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

  float targetHue;

  float getRandomFloat()
  {
    return random(999)/1000.0f;
  };

  void explodeVelocities() 
  {
    for (int i = 0; i < N; ++i) 
    {
      float randomWithDirection = (getRandomFloat() * 2.0f) - 1.0f;
      velocities[i] = 0.085f * randomWithDirection;
    }
  }

  void reset()
  {
    explodeVelocities();
    targetHue = getRandomFloat();
  }

public:
  PixelFade() 
  {
    float startHue = getRandomFloat();
    memset(&hues, startHue, sizeof(float) * N);

    reset();
  };
  
  void updateAndRender(Strip& strip, float smallKnobValue)
  {
    float brightness = smallKnobValue * 0.4f;

    // Seed random based on selected hue so that pixels have different
    // random speeds each time
    //randomSeed(targetHue *1200);

    float maxDifference = 0.0f;

    for (int i = 0; i < N; i++)
    {
      // Update velocities
      float distance = targetHue - hues[i];
      if (distance > 0.5f) distance = -1.0f - distance;
      if (distance < -0.5f)  distance = 1.0f + distance; 

      float attractionForce = distance * 0.0125f;
      velocities[i] += attractionForce;

      float damping = -velocities[i] * 0.011f;
      velocities[i] += damping;
      
      // Update hue with velocity
      hues[i] += velocities[i];
      while (hues[i] > 1.0f) hues[i] -= 1.0f;
      while (hues[i] < 0.0f) hues[i] += 1.0f;

      strip.setPixelHSVf(i, hues[i], 0.4f, brightness);

      maxDifference = max(maxDifference, abs(distance));
    }

    // When all pixels are close to the target hue 
    // select a new hue at random
    if(maxDifference < 0.01f)  
    {
      reset();
    }
  };

};

#endif