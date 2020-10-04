#include <Adafruit_NeoPixel.h>

#include "BlobWorld.h"

#define PIN 8
#define STRIPLENGTH 99.0f// Length of strip less 1
#define STRIPLENGTHINT 99// Length of strip less 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);

float brightness = 0.6f;

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
  float k = 0.0f;
  float delta1;
  float delta2;
  float delta3;
  int j = 0;

  BlobWorld blobWorld(STRIPLENGTHINT);

  float hue;

  // constant colour mode
  if (digitalRead(3) == HIGH)
  {
    Serial.print(analogRead(3));
    brightness = 1 - analogRead(3) / 1023.0f;
    hue = analogRead(0) / 1024.0f;

    for (int i = 0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i, brightness * (127 * cos(hue * 6.3) + 127), brightness * (127 * cos(hue * 6.3 + 4.2) + 127), brightness * (127 * cos(hue * 6.3 + 8.4) + 127));
    }

    strip.show();
  }

  // fun mode
  if (digitalRead(2) == HIGH)
  {
    // rainbow mode
    while (analogRead(0) < 90)
    {
      brightness = 1 - analogRead(3) / 1023.0f;
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

    // white mode
    while ((analogRead(0) >= 90) && (analogRead(0) < 150))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * 255, brightness * 255, brightness * 255);
      }
      strip.show();
    }

    // warm white mode
    while ((analogRead(0) >= 150) && (analogRead(0) < 210))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * 41, brightness * 255, brightness * 147);
      }
      strip.show();
    }

    // triangle strobe mode
    while ((analogRead(0) >= 210) && (analogRead(0) < 341))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * (255 * k), brightness * (255 * k), brightness * (255 * k));
      }
      strip.show();

      if (k <= 1)
      {
        k = k + 0.05;
      }
      else
      {
        k = 0;
      }
    }

    // normal strobe mode
    while ((analogRead(0) >= 341) && (analogRead(0) < 457))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * 255, brightness * 255, brightness * 255);
      }
      strip.show();
      delay(10);

      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * 0, brightness * 0, brightness * 0);
      }
      strip.show();
      delay(10);
    }

    // constant colour mode
    while ((analogRead(0) >= 454) && (analogRead(0) < 568))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness * 255, brightness * 200, brightness * 100);
      }
      strip.show();
      delay(50);
    }

    // overflowing mode
    while ((analogRead(0) >= 568) && (analogRead(0) < 682))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
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

    // some sort of interference pattern thing
    while ((analogRead(0) >= 682) && (analogRead(0) < 796))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        //delta[1]=(cos(i*6.3f/49.0f*2.0f+k)/5.0f+0.66f+cos(i*6.3f/49.0f*10.0f+2*k)/12.5f+cos(i*6.3f/49.0f*18.2f+2*k)/17.5f);
        delta1 = (cos(i * 6.3f / STRIPLENGTH * 2.0f + k) / 5.0f + 0.66f);
        delta2 = (0.66f + cos(i * 6.3f / STRIPLENGTH * 10.0f + 2 * k) / 12.5f);
        delta3 = (0.66f + cos(i * 6.3f / STRIPLENGTH * 18.2f + 2 * k) / 17.5f);
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

    // swimming pool mode
    while ((analogRead(0) >= 796) && (analogRead(0) < 909))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      for (int i = 0; i <= STRIPLENGTHINT; i++)
      {
        delta1 = (cos(i * 6.3f / STRIPLENGTH * 2.0f + k) / 5.0f + 0.8f);
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

    // blob physics mode
    while ((analogRead(0) >= 909))
    {
      brightness = 1 - analogRead(3) / 1023.0f;
      blobWorld.update(strip, brightness);
      Serial.print(brightness);
    }
  }
}
