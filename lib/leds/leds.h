#ifndef Leds_h
#define Leds_h
#include "Arduino.h"

// https://github.com/FastLED/FastLED
#include "FastLED.h"

class Leds
{
public:
  Leds();
  void setup();
  void loop();
  void incrementPreset();

private:
  int8_t currentPreset;
  CRGBPalette16 currentPalette;
  TBlendType currentBlending;
  uint8_t startIndex;
  CRGB leds[];
  void FillLEDsFromPaletteColors(uint8_t);
  void pPolice();
  void pOrangePinkYellow();
};
#endif