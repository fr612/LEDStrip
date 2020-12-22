#ifndef EFFECT_HPP
#define EFFECT_HPP

class Strip;

class Effect
{
public:

  Effect(){};

  virtual void updateAndRender(Strip& strip, float smallKnobValue) = 0;

};

#endif