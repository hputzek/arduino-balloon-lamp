#ifndef Leds_h
#define Leds_h
#include "Arduino.h"
#include "EEPROM.h"

// https://github.com/FastLED/FastLED
#include "FastLED.h"

// https://github.com/alextaujenis/RBD_Timer
#include "RBD_Timer.h"

class Leds
{
public:
  Leds();
  void setup();
  void loop();
  void incrementPreset();
  void setFader1(uint8_t value);
  uint8_t getFader1();
  void setFader2(uint8_t value);
  uint8_t getFader2();
  void setBrightness(uint8_t value);
  void saveCurrentPreset();

private:
  void fillLEDsFromPaletteColors(uint8_t colorIndex);
  int8_t currentPreset;
  CRGBPalette16 currentPalette;
  TBlendType currentBlending;
  uint8_t startIndex;
  CRGB leds[];
  void paletteMode();
  void pPolice();
  void pFixedColorMode();
};
#endif