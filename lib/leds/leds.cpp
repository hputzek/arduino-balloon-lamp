#include "leds.h"

#include "gradients.h"

#define NUMBER_OF_LEDS 40
#define LED_TYPE WS2811
#define LED_DATA_PIN 5
#define LED_COLOR_ORDER GRB
#define UPDATES_PER_SECOND 80

CRGB leds[NUMBER_OF_LEDS];
CRGBPalette16 currentPalette;
TBlendType currentBlending;
uint8_t startIndex = 230;

Leds::Leds()
{
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection( TypicalSMD5050 );
    currentPalette = Pink_Yellow_Orange_1_gp;
    currentBlending = LINEARBLEND;
    FastLED.setBrightness(255);
};

void Leds::loop()
{
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors(startIndex);
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