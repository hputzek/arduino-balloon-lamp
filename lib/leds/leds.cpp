#include "leds.h"

#include "gradients.h"
#define NUMBER_OF_LEDS 40
#define LED_TYPE WS2811
#define LED_DATA_PIN 5
#define LED_COLOR_ORDER GRB
#define UPDATES_PER_SECOND 80

// led array
CRGB leds[NUMBER_OF_LEDS];
// currently active color palette
CRGBPalette16 currentPalette;
// currently active blending
TBlendType currentBlending;
// start index of current palette
uint8_t colorIndex = 230;
// Index number of which pattern is current
uint8_t currentPatternNumber = 0;
// rotating "base color" used by many of the patterns
int8_t rotateHue = 0;

// currently active preset
int8_t currentPreset = 0;

Leds::Leds()
{
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
    currentPalette = Pink_Yellow_Orange_1_gp;
    currentBlending = LINEARBLEND;
    FastLED.setBrightness(255);
};

void Leds::loop()
{
    switch (currentPreset)
    {
    case 0:
        this->pPolice();
        break;
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
};

void Leds::FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 10;
    }
}

void Leds::pPolice()
{
    if (NUMBER_OF_LEDS >= rotateHue)
    {
        leds[rotateHue] = CRGB::Blue;
        FastLED.delay(5);
    }
    else
    {
        rotateHue = 0;
        FastLED.clear();
    }
    leds[rotateHue -4] = CRGB::Black;
    rotateHue++;
}