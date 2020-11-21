#include "SpiderEyes.hpp"

#define ULONG_MAX 4294967295

const float pi = 3.14;

SpiderEyes::SpiderEyes(Adafruit_NeoPixel & strip)
{
    Eye mainEyeRight;
    Eye mainEyeLeft;

    // The default value is 2 so that's fine for the rest
    // of the strips
    mainEyeRight.length = 66;
    mainEyeLeft.length = 32;

    // set offsets so that the two triangles can be treated void as
    // separate 'eyes'
    mainEyeRight.firstPixelOffset = 0;
    mainEyeLeft.firstPixelOffset = 66;

    strip.show();

    eyes.push_back(mainEyeRight);
    eyes.push_back(mainEyeLeft);

    setGlobalBrightness(globalBrightness, strip);
}

void SpiderEyes::setPattern(PatternMode newPatternMode)
{
    patternMode = newPatternMode;
    switch (newPatternMode)
    {
        case PATTERN_CREEPY_EYES:
        {
            Serial.print("Setting Creepy Eyes\n");
            creepyEyesInit();
            break;
        }
        case PATTERN_ATTACK_EYES:
        {
            Serial.print("Setting Attack Eyes\n");
            //attackEyesInit();
            break;
        }
    }
}

void SpiderEyes::poll(Adafruit_NeoPixel & strip)
{
    switch (patternMode)
    {
        case PATTERN_OFF:
        {
            setGlobalBrightness(0, strip);
            break;
        }
        case PATTERN_CREEPY_EYES:
        {
            creepyEyesPoll(strip);
            break;
        }
        case PATTERN_ATTACK_EYES:
        {
            //attackEyesPoll();
            break;
        }
    }
}

// Don't use this to turn shit off
void SpiderEyes::setGlobalBrightness(uint8_t brightness, Adafruit_NeoPixel & strip)
{
    strip.setBrightness(brightness);
    //strip->show();

    globalBrightness = brightness;
}

void SpiderEyes::setPatternTime(int timeMs)
{
    if (animationTimeMs/10 <= 20)
    {
        animationTimeMs = 200;
    }
    else
    {
        animationTimeMs = timeMs;
    }
}

// Check if millis() has wrapped and handle wrapping of
// other variables if it has
void SpiderEyes::patternWrapCheckPoll()
{
    if (millis() < lastTimeMs)
    {
        lastTimeMs = ULONG_MAX - lastTimeMs;
        lastTimeMs = animationTimeMs - lastTimeMs;
    }
    if (millis() < lastFadeTimeMs)
    {
        lastFadeTimeMs = ULONG_MAX - lastFadeTimeMs;
        lastFadeTimeMs = animationTimeMs/5 - lastFadeTimeMs;
    }
}

void SpiderEyes::creepyEyesInit()
{
    //Serial.print("Creepy Eyes init\n");
    for (Eye eye : eyes)
    {
        eye.highlightLed = 1;
        eye.trailLed = 0;
    }
}

static void incrementOscillatingColour(uint8_t &colour, int8_t &increment, uint8_t oscillationAmplitude)
{
    if (colour + increment + oscillationAmplitude >= 255)
    {
        increment = -increment;
    }
    else if (colour + increment - oscillationAmplitude <= 0)
    {
        increment = -increment;
    }
    colour = colour + increment;
}

void SpiderEyes::creepyEyesPoll(Adafruit_NeoPixel & strip)
{
    // Add sine shit here
    float f = 0.3;
    float t = (float) millis()/1000;
    float arg = 2 * pi * f * t;

    // Need to handle overflow
    if (millis() - lastTimeMs >= animationTimeMs)
    {
        //Serial.print("LED switch condition hit\n");
        //Serial.print("animationTimeMs: ");
        //Serial.print(animationTimeMs);
        //Serial.print("\n");
        incrementOscillatingColour(mainLedCreepyRed, mainLedCreepyRedIncrement, 0);
        incrementOscillatingColour(mainLedCreepyGreen, mainLedCreepyGreenIncrement, 0);
        incrementOscillatingColour(mainLedCreepyBlue, mainLedCreepyBlueIncrement, 0);
        incrementOscillatingColour(mainLedHighlightRed, mainLedHighlightRedIncrement, 0);
        incrementOscillatingColour(mainLedHighlightGreen, mainLedHighlightGreenIncrement, 0);
        incrementOscillatingColour(mainLedHighlightBlue, mainLedHighlightBlueIncrement, 0);
        lastTimeMs = millis();

        for (Eye eye : eyes)
        {
            for (int pix = eye.firstPixelOffset; pix < eye.firstPixelOffset + eye.length; pix++)
            {
                if (pix == eye.highlightLed + eye.firstPixelOffset)
                {
                    strip.setPixelColor(pix, mainLedHighlightGreen, mainLedHighlightRed, mainLedHighlightBlue);
                }
                else
                {
                    strip.setPixelColor(pix, mainLedCreepyGreen, mainLedCreepyRed, mainLedCreepyBlue);
                }
            }

            // wrap the highlight LED
            eye.highlightLed = (eye.highlightLed >= (eye.firstPixelOffset + eye.length))
                               ? 0
                               : eye.highlightLed + 1;
        }

        redFadeAmount = (mainLedHighlightRed - mainLedCreepyRed)/10;
        greenFadeAmount = (mainLedHighlightGreen - mainLedCreepyGreen)/10;
        blueFadeAmount = (mainLedHighlightBlue - mainLedCreepyBlue)/10;
        fadeCoefficient = 1;
    }

    // Do some dope-ass fading
    // TODO make variable for fade increment
    if ((millis() - lastFadeTimeMs) >= animationTimeMs/10)
    {
        //Serial.print("Last Fade Time: ");
        //Serial.print(lastFadeTimeMs);
        //Serial.print("\n");
        lastFadeTimeMs = millis();
        uint8_t redHighlight = mainLedCreepyRed + fadeCoefficient*redFadeAmount;
        uint8_t greenHighlight = mainLedCreepyGreen + fadeCoefficient*greenFadeAmount;
        uint8_t blueHighlight = mainLedCreepyBlue + fadeCoefficient*blueFadeAmount;
        uint8_t redTrail = mainLedHighlightRed - fadeCoefficient*redFadeAmount;
        uint8_t greenTrail = mainLedHighlightGreen - fadeCoefficient*greenFadeAmount;
        uint8_t blueTrail = mainLedHighlightBlue - fadeCoefficient*blueFadeAmount;
        char colourOutput[50];
        sprintf(colourOutput, "Highlight RGB: %d %d %d, Trail RGB %d %d %d\n", redHighlight, greenHighlight, blueHighlight, redTrail, greenTrail, blueTrail);

        // Set the highlight colours
        for (Eye eye : eyes)
        {
            strip.setPixelColor(eye.highlightLed, greenHighlight, redHighlight, blueHighlight);
            strip.setPixelColor(eye.trailLed, greenTrail, redTrail, blueTrail);
        }

        fadeCoefficient++;
    }

    strip.show();
}
