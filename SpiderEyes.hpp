#ifndef SPIDEREYES_HPP
#define SPIDEREYES_HPP

#include <Adafruit_NeoPixel.h>

#include <Vector.h>

// Wraps up the NeoPixel object and it's current RGB values into one
// object for convenience
class Eye
{
public:
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t highlightLed = 0;
    uint8_t trailLed = 0;
    uint8_t highlightLedBrightness = 0;
    uint8_t firstPixelOffset = 0;
    uint8_t length = 2;
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
    SpiderEyes(Adafruit_NeoPixel & strip);

    void setPattern(PatternMode newPatternMode);
    void setGlobalBrightness(uint8_t brightness, Adafruit_NeoPixel & strip);
    void setPatternTime(int timeMs);

    void poll(Adafruit_NeoPixel & strip);

    // Global brightness setting
    uint8_t globalBrightness = 150;

    // Initialise creepy eyes pattern
    void creepyEyesInit();

private:

    void setEyeRGB(Eye *eye, uint8_t red, uint8_t green, uint8_t blue);

    void creepyEyesPoll(Adafruit_NeoPixel & strip);

    void patternWrapCheckPoll();

    Vector<Eye> eyes;

    Adafruit_NeoPixel * strip;

    // Watch that the fade increment isn't too short
    // 10ms is definitely too short
    int animationTimeMs = 500;
    int attackEyesTimeMs = 100;

    // Used to track how long it has been since the last action
    unsigned long lastTimeMs = 0;
    unsigned long lastFadeTimeMs = 0;

    // Amount to fade between colours
    int8_t redFadeAmount = 0;
    int8_t greenFadeAmount = 0;
    int8_t blueFadeAmount = 0;
    uint8_t fadeCoefficient = 0;

    // Colours for Creepy Eyes
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
