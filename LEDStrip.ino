#include <Adafruit_NeoPixel.h>

#include "BlobWorld.h"

#define PIN 8

#define STRIPLENGTH 97.0f// Length of strip less 1
#define STRIPLENGTHINT 97// Length of strip less 1

#define BIG_KNOB_PIN 0
#define SMALL_KNOB_PIN 3

enum FunModeEffects {
  STROBE_SQUARE,
  INTERFERENCE_PATTERN,
  SWIMMING_POOL,
  RAINBOW,
  OVERFLOWING,
  BLOBS,

  // Place new effects above this point
  // Any below this will be disabled
  NUM_EFFECTS,

  STROBE_TRIANGLE,
  WHITE,
  WHITE_WARM,
  CONSTANT_COLOR
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(98, PIN, NEO_GRB + NEO_KHZ800);

BlobWorld blobWorld(STRIPLENGTHINT);
float k = 0.0f;

float readBigKnob() {
  // 1 - result because box is upside down 
  return 1 - ((analogRead(BIG_KNOB_PIN) + 1) / 1024.0f);
}

float readSmallKnob() {
  // 1 - result because box is upside down 
  return 1 - ((analogRead(SMALL_KNOB_PIN) + 1) / 1024.0f);
}

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(3) == HIGH) updateConstantColorMode();
  if (digitalRead(2) == HIGH) updateFunMode();
}

void updateConstantColorMode()
{
  float brightness = readSmallKnob();
  float hue = readBigKnob();

  for (int i = 0; i <= STRIPLENGTHINT; i++)
  {
    strip.setPixelColor(i, brightness * (127 * cos(hue * 6.3) + 127), brightness * (127 * cos(hue * 6.3 + 4.2) + 127), brightness * (127 * cos(hue * 6.3 + 8.4) + 127));
  }

  strip.show();
}

void updateFunMode()
{
  int selectedEffect = readBigKnob() * NUM_EFFECTS;

  if (selectedEffect == RAINBOW)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + k) + 127), brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + 2.1 + k) + 127), brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + 4.2 + k) + 127));
    }
    strip.show();

    if (k <= 6.3)
    {
      k = k + 0.1;
    }
    else
    {
      k = 0;
    }
  }

  if (selectedEffect == WHITE)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * 255, brightness * 255, brightness * 255);
    }
    strip.show();
  }

  if (selectedEffect == WHITE_WARM)
  {
    float brightness = 1 - readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * 41, brightness * 255, brightness * 147);
    }
    strip.show();
  }

  if (selectedEffect == STROBE_TRIANGLE)
  {
    float brightness = 0.7f;
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * (255 * k), brightness * (255 * k), brightness * (255 * k));
    }
    strip.show();

    if (k <= 1)
    {
      k = k + 0.1 * readSmallKnob();
    }
    else
    {
      k = 0;
    }
  }

  if (selectedEffect == STROBE_SQUARE)
  {
    float brightness = 0.6f;
    int delayLength = min(10/readSmallKnob(), 1000);

    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * 255, brightness * 255, brightness * 255);
    }
    strip.show();
    delay(delayLength);

    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * 0, brightness * 0, brightness * 0);
    }
    strip.show();
    delay(delayLength);
  }

  if (selectedEffect == CONSTANT_COLOR)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * 255, brightness * 200, brightness * 100);
    }
    strip.show();
    delay(50);
  }

  if (selectedEffect == OVERFLOWING)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + k) + 127) * 3, brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + 2.1 + k) + 127) * 3, brightness * (127 * cos(i * 6.3 / STRIPLENGTHINT + 4.2 + k) + 127) * 3);
    }
    strip.show();

    if (k <= 6.3)
    {
      k = k + 0.1;
    }
    else
    {
      k = 0;
    }
  }

  if (selectedEffect == INTERFERENCE_PATTERN)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      //delta[1]=(cos(i*6.3f/49.0f*2.0f+k)/5.0f+0.66f+cos(i*6.3f/49.0f*10.0f+2*k)/12.5f+cos(i*6.3f/49.0f*18.2f+2*k)/17.5f);
      float delta1 = (cos(i * 6.3f / STRIPLENGTH * 2.0f + k) / 5.0f + 0.66f);
      float delta2 = (0.66f + cos(i * 6.3f / STRIPLENGTH * 10.0f + 2 * k) / 12.5f);
      float delta3 = (0.66f + cos(i * 6.3f / STRIPLENGTH * 18.2f + 2 * k) / 17.5f);
      strip.setPixelColor(i, delta1 * brightness * 41, brightness * 255 * delta2, brightness * 147 * delta3);
    }
    strip.show();

    if (k <= 6.3f)
    {
      k = k + 0.05f + random(0, 100) / 2000.0f;
    }
    else
    {
      k = 0.0f;
    }
  }

  if (selectedEffect == SWIMMING_POOL)
  {
    float brightness = readSmallKnob();
    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      float delta1 = (cos(i * 6.3f / STRIPLENGTH * 2.0f + k) / 5.0f + 0.8f);
      strip.setPixelColor(i, brightness * delta1 * 41, brightness * delta1 * 255, brightness * delta1 * 147);
    }
    strip.show();

    if (k <= 6.3f)
    {
      k = k + 0.15f;
    }
    else
    {
      k = 0.0f;
    }

    Serial.print(k);
  }

  if (selectedEffect == BLOBS)
  {
    float brightness = readSmallKnob();
    blobWorld.update(strip, brightness);
  }
}
