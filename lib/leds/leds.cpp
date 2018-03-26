#include "leds.h"
#include "gradients.h"
#define NUMBER_OF_LEDS 40
#define NUMBER_OF_PRESETS 7
#define LED_TYPE WS2811
#define LED_DATA_PIN 5
#define LED_COLOR_ORDER GRB
#define UPDATES_PER_SECOND 60

// EEPROM adresses
#define EEPROM_PRESET 0
#define EEPROM_BRIGHTNESS 1
#define EEPROM_COLOR_INDEX 2
#define EEPROM_ROTATE_HUE 3

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
uint8_t brightness = 128;

uint8_t speed = 100;

// currently active preset
uint8_t currentPreset = 0;

struct preset
{
    uint8_t fader1;
    uint8_t fader2;
};

// Array of structs holding preset data
preset presets[NUMBER_OF_PRESETS];

// Timers used in conjunction with the faders
RBD::Timer timer1(200);
RBD::Timer timer2(200);

Leds::Leds()
{
    // restore state
    EEPROM.begin();
    brightness = EEPROM.read(EEPROM_BRIGHTNESS);
    uint8_t loadedPreset = EEPROM.read(EEPROM_PRESET);
    if (loadedPreset > NUMBER_OF_PRESETS)
    {
        currentPreset = 0;
    }
    else
    {
        currentPreset = loadedPreset;
    }
    colorIndex = EEPROM.read(EEPROM_COLOR_INDEX);
    rotateHue = EEPROM.read(EEPROM_ROTATE_HUE);

    // setup FASTLED
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
    currentPalette = Pink_Yellow_Orange_1_gp;
    currentBlending = LINEARBLEND;
    FastLED.setBrightness(brightness);
};

void Leds::loop()
{
    // handle presets
    switch (currentPreset)
    {
    case 0:
        this->pFixedColorMode();
        break;
    case 1:
        currentPalette = trove_gp;
        this->paletteMode();
        break;
    case 2:
        currentPalette = patriot_gp;
        this->paletteMode();
        break;
    case 3:
        currentPalette = Fire_2_gp;
        this->paletteMode();
        break;
    case 4:
        currentPalette = Fuschia_6_gp;
        this->paletteMode();
        break;
    case 5:
        currentPalette = Pink_Yellow_Orange_1_gp;
        this->paletteMode();
        break;
    case 6:
        this->pPolice();
        break;
    }
    FastLED.show();
    // handle timers
    if (timer1.isExpired())
    {
        timer1.restart();
    }
    if (timer2.isExpired())
    {
        timer2.restart();
    }
};

void Leds::incrementPreset()
{
    if (currentPreset == 0)
    {
        EEPROM.update(EEPROM_BRIGHTNESS, brightness);
    }
    if (currentPreset < NUMBER_OF_PRESETS - 1)
    {
        currentPreset++;
    }
    else
    {
        currentPreset = 0;
    }
}

void Leds::fillLEDsFromPaletteColors(uint8_t colorIndex)
{
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        CRGB color = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        CHSV hsvColor = rgb2hsv_approximate(color);
        hsvColor.hue += rotateHue;
        leds[i] = hsvColor;
        leds[i].maximizeBrightness();
        colorIndex += (255 / NUMBER_OF_LEDS);
    }
}

uint8_t Leds::getFader1()
{
    return presets[currentPreset].fader1;
}

void Leds::setFader1(uint8_t value)
{
    presets[currentPreset].fader1 = constrain(value, 1, 255);
    timer1.setTimeout(presets[currentPreset].fader1 / 2);
}

uint8_t Leds::getFader2()
{
    return presets[currentPreset].fader2;
}

void Leds::setFader2(uint8_t value)
{
    presets[currentPreset].fader2 = constrain(value, 1, 255);
    timer2.setTimeout(presets[currentPreset].fader2 / 2);
}

void Leds::setBrightness(uint8_t value)
{
    brightness = constrain(value, 0, 255);
    FastLED.setBrightness(brightness);
}

uint8_t Leds::getSavedBrightness()
{
    return EEPROM.read(EEPROM_BRIGHTNESS);
}

void Leds::saveCurrentPresetAsDefault()
{
    EEPROM.update(EEPROM_PRESET, currentPreset);
    EEPROM.update(EEPROM_COLOR_INDEX, colorIndex);
    EEPROM.update(EEPROM_ROTATE_HUE, rotateHue);
}

void Leds::blinkSuccess()
{
    FastLED.setBrightness(255);
    for (uint8_t i = 0; i < 3; i++)
    {
        fill_solid(leds, NUMBER_OF_LEDS, CRGB::Green);
        FastLED.delay(200);
        FastLED.clear();
        FastLED.delay(150);
    }
    FastLED.setBrightness(brightness);
}

void Leds::showBatteryLevel(uint8_t percentage)
{
    FastLED.setBrightness(255);
    uint8_t mappedPercentage = map(percentage, 0, 100, 0, 255);
    fill_solid(leds, NUMBER_OF_LEDS, CRGB(255 - mappedPercentage, mappedPercentage, 0));
    FastLED.delay(2000);
    FastLED.setBrightness(brightness);
}

void Leds::pPolice()
{
    if (NUMBER_OF_LEDS >= progress)
    {
        leds[progress] = CRGB::Blue;
        FastLED.delay(20);
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

void Leds::pFixedColorMode()
{
    CHSV hsvColor;
    hsvColor.setHSV(getFader1(), 255, 255);
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        leds[i] = hsvColor;
    }
    setBrightness(getFader2());
}

void Leds::paletteMode()
{
    if (timer1.isExpired())
    {
        rotateHue++;
    }
    if (timer2.isExpired())
    {
        colorIndex++;
    }
    if (timer1.isExpired() || timer2.isExpired())
    {
        fillLEDsFromPaletteColors(colorIndex);
    }
}