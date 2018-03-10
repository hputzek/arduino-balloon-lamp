#include "leds.h"
#include "gradients.h"
#define NUMBER_OF_LEDS 40
#define NUMBER_OF_PRESETS 2
#define LED_TYPE WS2811
#define LED_DATA_PIN 5
#define LED_COLOR_ORDER GRB
#define UPDATES_PER_SECOND 60

// led array
CRGB leds[NUMBER_OF_LEDS];
// currently active color palette
CRGBPalette16 currentPalette;
// currently active blending
TBlendType currentBlending;
// start index of current palette
uint8_t colorIndex = 0;
// rotating "base color" used by many of the patterns
uint8_t rotateHue = 0;
// progress of current pattern
int progress = 0;
// overall brightness
uint8_t brightness = 255;

uint8_t speed = 100;

// currently active preset
uint8_t currentPreset = 0;

struct preset {
    uint8_t fader1;
    uint8_t fader2;
};

// Array of structs holding preset data
preset presets[NUMBER_OF_PRESETS];



Leds::Leds()
{
    // setup FASTLED
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
    //currentPalette = Pink_Yellow_Orange_1_gp;
    currentBlending = LINEARBLEND;
    FastLED.setBrightness(brightness);
};

void Leds::loop()
{
    // handle presets
    switch (currentPreset)
    {
    case 0:
        this->pPolice();
        break;
    case 1:
        this->pOrangePinkYellow();
        break;
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
};

void Leds::incrementPreset() {
     if(currentPreset < NUMBER_OF_PRESETS -1) {
         currentPreset ++;
     } else {
         currentPreset = 0;
     }
}

void Leds::FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += (255/NUMBER_OF_LEDS);
    }
}

void Leds::pPolice()
{
    if (NUMBER_OF_LEDS >= progress)
    {
        leds[progress] = CRGB::Blue;
        FastLED.delay(5);
    }
    else
    {
        progress = 0;
        FastLED.clear();
        leds[progress] = CRGB::Blue;
    }
    leds[progress - 4] = CRGB::Black;
    progress++;
}

void Leds::pOrangePinkYellow()
{
    currentPalette = Pink_Yellow_Orange_1_gp;
    FillLEDsFromPaletteColors(colorIndex);
    colorIndex += (255/40);
    FastLED.delay(10);
}