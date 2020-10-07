#include <Adafruit_NeoPixel.h>

#define PIN 8
#define STRIPLENGTH 99.0f// Length of strip less 1
#define STRIPLENGTHINT 99// Length of strip less 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);

int pixelIndex = 0;

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
  int nextPixelIndex = (pixelIndex + 1) % STRIPLENGTHINT;

  strip.setPixelColor(pixelIndex,0,0,0);
  strip.setPixelColor(nextPixelIndex, 255, 255, 255);
  strip.show();

  pixelIndex = nextPixelIndex;

  delay(1000);
}
