#include "SpiderEyes.hpp"

#define ULONG_MAX 4294967295

const float pi = 3.14;

SpiderEyes::SpiderEyes()
{
    // Initialise the strips in the structs
    frontEye1.strip = new Adafruit_NeoPixel(2, 5);
    frontEye2.strip = new Adafruit_NeoPixel(2, 3);
    frontEye3.strip = new Adafruit_NeoPixel(2, 8);
    frontEye4.strip = new Adafruit_NeoPixel(2, 2);
    sideEyeLeft.strip = new Adafruit_NeoPixel(2, 6);
    sideEyeRight.strip = new Adafruit_NeoPixel(2, 4);
    mainEyeLeft.strip = new Adafruit_NeoPixel(6, 7);
    mainEyeRight.strip = new Adafruit_NeoPixel(6, 9);

    // The default value is 2 so that's fine for the rest
    // of the strips
    mainEyeLeft.length = 6;
    mainEyeRight.length = 6;

    // Initialise the strips
    frontEye4.strip->begin();
    frontEye2.strip->begin();
    sideEyeRight.strip->begin();
    frontEye1.strip->begin();
    sideEyeLeft.strip->begin();
    mainEyeLeft.strip->begin();
    frontEye3.strip->begin();
    mainEyeRight.strip->begin();

    // Make sure all the strips are off
    frontEye4.strip->show();
    frontEye2.strip->show();
    sideEyeRight.strip->show();
    frontEye1.strip->show();
    sideEyeLeft.strip->show();
    mainEyeLeft.strip->show();
    frontEye3.strip->show();
    mainEyeRight.strip->show();

    setGlobalBrightness(globalBrightness);
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
            attackEyesInit();
            break;
        }
    }
}

void SpiderEyes::Poll()
{
    switch (patternMode)
    {
        case PATTERN_OFF:
        {
            setGlobalBrightness(0);
            break;
        }
        case PATTERN_CREEPY_EYES:
        {
            creepyEyesPoll();
            break;
        }
        case PATTERN_ATTACK_EYES:
        {
            attackEyesPoll();
            break;
        }
    }
}

void SpiderEyes::setEyeRGB(Eye *eye, uint8_t red, uint8_t green, uint8_t blue)
{
    eye->red = red;
    eye->green = green;
    eye->blue = blue;

    for (int i = 0; i < eye->length; i++)
    {
        // The function takes GRB for some reason...
        eye->strip->setPixelColor(i, green, red, blue);
    }
    eye->strip->show();
}

// Don't use this to turn shit off
void SpiderEyes::setGlobalBrightness(uint8_t brightness)
{
    frontEye1.strip->setBrightness(brightness);
    frontEye2.strip->setBrightness(brightness);
    frontEye3.strip->setBrightness(brightness);
    frontEye4.strip->setBrightness(brightness);
    sideEyeRight.strip->setBrightness(brightness);
    sideEyeLeft.strip->setBrightness(brightness);
    mainEyeLeft.strip->setBrightness(brightness);
    mainEyeRight.strip->setBrightness(brightness);

    frontEye1.strip->show();
    frontEye2.strip->show();
    frontEye3.strip->show();
    frontEye4.strip->show();
    sideEyeRight.strip->show();
    sideEyeLeft.strip->show();
    mainEyeLeft.strip->show();
    mainEyeRight.strip->show();

    globalBrightness = brightness;
}

void SpiderEyes::Strobe(bool on)
{
    if (on)
    {
        setEyeRGB(&frontEye1, 255, 0, 0);
        setEyeRGB(&frontEye2, 255, 0, 0);
        setEyeRGB(&frontEye3, 255, 0, 0);
        setEyeRGB(&frontEye4, 255, 0, 0);
        setEyeRGB(&sideEyeRight, 255, 0, 0);
        setEyeRGB(&sideEyeLeft, 255, 0, 0);
        setEyeRGB(&mainEyeLeft, 255, 0, 0);
        setEyeRGB(&mainEyeRight, 255, 0, 0);
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            frontEye1.strip->setPixelColor(i, 0, 0, 0);
            frontEye2.strip->setPixelColor(i, 0, 0, 0);
            frontEye3.strip->setPixelColor(i, 0, 0, 0);
            frontEye4.strip->setPixelColor(i, 0, 0, 0);
            sideEyeRight.strip->setPixelColor(i, 0, 0, 0);
            sideEyeLeft.strip->setPixelColor(i, 0, 0, 0);
        }

        for (int i = 0; i < 6; i++)
        {
            mainEyeLeft.strip->setPixelColor(i, 0, 0, 0);
            mainEyeRight.strip->setPixelColor(i, 0, 0, 0);
        }

        frontEye1.strip->show();
        frontEye2.strip->show();
        frontEye3.strip->show();
        frontEye4.strip->show();
        sideEyeRight.strip->show();
        sideEyeLeft.strip->show();
        mainEyeLeft.strip->show();
        mainEyeRight.strip->show();
    }
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
    if (millis() < attackEyesLastTimeMs)
    {
        attackEyesLastTimeMs = ULONG_MAX - attackEyesLastTimeMs;
        attackEyesLastTimeMs = attackEyesTimeMs - attackEyesLastTimeMs;
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
    mainEyeLeft.highlightLed = 0;
    mainEyeLeft.trailLed = 5;
    mainEyeRight.highlightLed = 0;
    mainEyeRight.trailLed = 1;
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

void SpiderEyes::creepyEyesPoll()
{
    // Add sine shit here
    float f = 0.3;
    float t = (float) millis()/1000;
    float arg = 2 * pi * f * t;

    setEyeRGB(&frontEye1, smallLedCreepyRed + 30*sin(0.9 * arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 20*cos(arg));
    setEyeRGB(&frontEye2, smallLedCreepyRed + 30*sin(arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 18*cos(0.7 * arg));
    setEyeRGB(&frontEye3, smallLedCreepyRed + 30*sin(0.5 * arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 14*cos(arg));
    setEyeRGB(&frontEye4, smallLedCreepyRed + 30*sin(arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 25*cos(1.2 * arg));
    setEyeRGB(&sideEyeRight, smallLedCreepyRed + 30*sin(0.3 * arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 30*cos(arg));
    setEyeRGB(&sideEyeLeft, smallLedCreepyRed + 30*sin(arg), smallLedCreepyGreen + 5*sin(arg), smallLedCreepyBlue + 10*cos(arg));

    // Need to handle overflow
    if (millis() - lastTimeMs >= animationTimeMs)
    {
        //Serial.print("LED switch condition hit\n");
        //Serial.print("animationTimeMs: ");
        //Serial.print(animationTimeMs);
        //Serial.print("\n");
        incrementOscillatingColour(smallLedCreepyRed, smallLedCreepyRedIncrement, 30);
        incrementOscillatingColour(smallLedCreepyGreen, smallLedCreepyGreenIncrement, 5);
        incrementOscillatingColour(smallLedCreepyBlue, smallLedCreepyBlueIncrement, 30);
        incrementOscillatingColour(mainLedCreepyRed, mainLedCreepyRedIncrement, 0);
        incrementOscillatingColour(mainLedCreepyGreen, mainLedCreepyGreenIncrement, 0);
        incrementOscillatingColour(mainLedCreepyBlue, mainLedCreepyBlueIncrement, 0);
        incrementOscillatingColour(mainLedHighlightRed, mainLedHighlightRedIncrement, 0);
        incrementOscillatingColour(mainLedHighlightGreen, mainLedHighlightGreenIncrement, 0);
        incrementOscillatingColour(mainLedHighlightBlue, mainLedHighlightBlueIncrement, 0);
        lastTimeMs = millis();
        for (int i = 0; i < 6; i++)
        {
            if (i == mainEyeLeft.highlightLed)
            {
                mainEyeLeft.strip->setPixelColor(i, mainLedHighlightGreen, mainLedHighlightRed, mainLedHighlightBlue);
            }
            else
            {
                mainEyeLeft.strip->setPixelColor(i, mainLedCreepyGreen, mainLedCreepyRed, mainLedCreepyBlue);
            }
            if (i == mainEyeRight.highlightLed)
            {
                mainEyeRight.strip->setPixelColor(i, mainLedHighlightGreen, mainLedHighlightRed, mainLedHighlightBlue);
            }
            else
            {
                mainEyeRight.strip->setPixelColor(i, mainLedCreepyGreen, mainLedCreepyRed, mainLedCreepyBlue);
            }
        }
        // Wrap the left eye
        mainEyeLeft.highlightLed = (mainEyeLeft.highlightLed >= 5) ?
            0 : mainEyeLeft.highlightLed + 1;
        mainEyeLeft.trailLed = (mainEyeLeft.trailLed >= 5) ?
            0 : mainEyeLeft.trailLed + 1;
        //Serial.print("Main Left Highlight: ");
        //Serial.print(mainEyeLeft.highlightLed);
        //Serial.print("Main Left Trail: ");
        //Serial.print(mainEyeLeft.trailLed);
        //Serial.print("\n");
        // Wrap the right eye
        mainEyeRight.highlightLed = (mainEyeRight.highlightLed == 0) ?
            5 : mainEyeRight.highlightLed - 1;
        mainEyeRight.trailLed = (mainEyeRight.trailLed == 0) ?
            5 : mainEyeRight.trailLed - 1;
        //Serial.print("Main Right Highlight: ");
        //Serial.print(mainEyeRight.highlightLed);
        //Serial.print("Main Right Trail: ");
        //Serial.print(mainEyeRight.trailLed);
        //Serial.print("\n\n");

        redFadeAmount = (mainLedHighlightRed - mainLedCreepyRed)/10;
        greenFadeAmount = (mainLedHighlightGreen - mainLedCreepyGreen)/10;
        blueFadeAmount = (mainLedHighlightBlue - mainLedCreepyBlue)/10;
        fadeCoefficient = 1;

        char colourOutput[50];
        //Serial.print(colourOutput);
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
        //Serial.print(colourOutput);
        // Set the highlight colours
        mainEyeLeft.strip->setPixelColor(mainEyeLeft.highlightLed, greenHighlight, redHighlight, blueHighlight);
        mainEyeRight.strip->setPixelColor(mainEyeRight.highlightLed, greenHighlight, redHighlight, blueHighlight);
        // Set the trail colours
        mainEyeLeft.strip->setPixelColor(mainEyeLeft.trailLed, greenTrail, redTrail, blueTrail);
        mainEyeRight.strip->setPixelColor(mainEyeRight.trailLed, greenTrail, redTrail, blueTrail);
        fadeCoefficient++;
        //Serial.print("Fade Coefficient: ");
        //Serial.print(fadeCoefficient);
        //Serial.print("\n");
    }
    mainEyeRight.strip->show();
    mainEyeLeft.strip->show();
}

void SpiderEyes::attackEyesInit()
{
    //Serial.print("Attack Eyes\n");

    setEyeRGB(&frontEye1, 255, 0, 0);
    setEyeRGB(&frontEye2, 255, 0, 0);
    setEyeRGB(&frontEye3, 255, 0, 0);
    setEyeRGB(&frontEye4, 255, 0, 0);
    setEyeRGB(&sideEyeRight, 255, 0, 0);
    setEyeRGB(&sideEyeLeft, 255, 0, 0);
    setEyeRGB(&mainEyeRight, 255, 0, 0);
    setEyeRGB(&mainEyeLeft, 255, 0, 0);

    attackEyesLastTimeMs = millis() - attackEyesTimeMs;
}

void SpiderEyes::attackEyesPoll()
{
    if (millis() - attackEyesLastTimeMs >= attackEyesTimeMs)
    {
        attackEyesLastTimeMs = millis();
        switch (attackEyesLastStrobe)
        {
            case false:
            {
                Strobe(false);
                attackEyesLastStrobe = true;
                break;
            }
            case true:
            {
                Strobe(true);
                attackEyesLastStrobe = false;
                break;
            }
        }
    }
}
