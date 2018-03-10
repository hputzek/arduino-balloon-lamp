/*
	A LED Contnroller using Arduino for WS2811 LED's.
  Used for a balloon shaped lamp I will build with my nephew.

  Refer to github repo for more information.

  Created 11/2017 by Hendrik Putzek

*/

// https://github.com/mathertel/OneButton/
#include "OneButton.h"

// https://github.com/rocketscream/Low-Power/
// #include "LowPower.h"

#include "leds.h"

#define BUTTON1_PIN A2
#define INVERT true

#define FADER_1_PIN 0
#define FADER_2_PIN 1

Leds *balloonLeds;

// init control button
OneButton button1(BUTTON1_PIN, INVERT);

void handleButton1Click() {
  balloonLeds->incrementPreset();
}

void setup()
{
  balloonLeds = new Leds();
  //Serial.begin(9600);
  // handle buttons
  button1.attachClick(handleButton1Click);
}

void loop()
{
  // handle IO
  button1.tick();
  // handle leds
  balloonLeds->loop();
}
