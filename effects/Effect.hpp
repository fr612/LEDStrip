#ifndef EFFECT_HPP
#define EFFECT_HPP

#include "../Strip.hpp"

class Effect
{
public:
  Effect(Strip& strip) : 
    strip(strip) 
  {
  }

  virtual void updateAndRender(float smallKnobValue) = 0;

protected:
  Strip& strip;
};

#endif