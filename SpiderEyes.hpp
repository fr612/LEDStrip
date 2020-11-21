#ifndef SPIDEREYES_HPP
#define SPIDEREYES_HPP

#include <Adafruit_NeoPixel.h>

// Wraps up the NeoPixel object and it's current RGB values into one
// object for convenience
class Eye
{
public:
    Adafruit_NeoPixel * strip;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t highlightLed = 0;
    uint8_t trailLed = 0;
    uint8_t length = 2;
    uint8_t highlightLedBrightness = 0;

private:


};

// Which pattern the spider is in
enum PatternMode
{
    PATTERN_OFF = 0,
    PATTERN_CREEPY_EYES,
    PATTERN_ATTACK_EYES
};

class SpiderEyes
{
public:
    SpiderEyes();

    void setPattern(PatternMode newPatternMode);
    void setGlobalBrightness(uint8_t brightness);
    void setPatternTime(int timeMs);

    void Poll();

    // Global brightness setting
    uint8_t globalBrightness = 150;

    // Initialise creepy eyes pattern
    void creepyEyesInit();

private:

    void setEyeRGB(Eye *eye, uint8_t red, uint8_t green, uint8_t blue);

    void creepyEyesPoll();

    void attackEyesInit();
    void attackEyesPoll();

    void patternWrapCheckPoll();

    void Strobe(bool on);

    Eye frontEye1;
    Eye frontEye2;
    Eye frontEye3;
    Eye frontEye4;
    Eye sideEyeLeft;
    Eye sideEyeRight;
    Eye mainEyeLeft;
    Eye mainEyeRight;

    // Watch that the fade increment isn't too short
    // 10ms is definitely too short
    int animationTimeMs = 500;
    int attackEyesTimeMs = 100;

    // Used to track how long it has been since the last action
    unsigned long lastTimeMs = 0;
    unsigned long lastFadeTimeMs = 0;
    unsigned long attackEyesLastTimeMs = 0;
    bool attackEyesLastStrobe = false;

    // Amount to fade between colours
    int8_t redFadeAmount = 0;
    int8_t greenFadeAmount = 0;
    int8_t blueFadeAmount = 0;
    uint8_t fadeCoefficient = 0;

    // Colours for Creepy Eyes
    uint8_t smallLedCreepyRed = 220;
    int8_t smallLedCreepyRedIncrement = 1;
    uint8_t smallLedCreepyGreen = 20;
    int8_t smallLedCreepyGreenIncrement = -2;
    uint8_t smallLedCreepyBlue = 60;
    int8_t smallLedCreepyBlueIncrement = 3;
    uint8_t mainLedCreepyRed = 20;
    int8_t mainLedCreepyRedIncrement = -1;
    uint8_t mainLedCreepyGreen = 220;
    int8_t mainLedCreepyGreenIncrement = 2;
    uint8_t mainLedCreepyBlue = 60;
    int8_t mainLedCreepyBlueIncrement = -3;
    uint8_t mainLedHighlightRed = 255;
    int8_t mainLedHighlightRedIncrement = 3;
    uint8_t mainLedHighlightGreen = 100;
    int8_t mainLedHighlightGreenIncrement = -1;
    uint8_t mainLedHighlightBlue = 0;
    int8_t mainLedHighlightBlueIncrement = 3;

    PatternMode patternMode = PATTERN_OFF;
};

#endif // Spider Eyes
