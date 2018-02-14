#include "leds.h";
// https://github.com/FastLED/FastLED
#include "FastLED.h";

#define NUMBER_OF_LEDS 40
#define LED_TYPE WS2811
#define LED_DATA_PIN 5
#define LED_COLOR_ORDER GRB



Leds::Leds()
{
    struct CRGB leds[NUMBER_OF_LEDS];
    LEDS.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUMBER_OF_LEDS);
    FastLED.setBrightness(255);
};

void Leds::loop()
{
    FastLED.showColor(CRGB(random8(255),random8(255),random8(255)));
    delay(1000);
};