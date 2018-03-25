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

// https://github.com/dxinteractive/ResponsiveAnalogRead
#include "ResponsiveAnalogRead.h"

// https://github.com/alextaujenis/RBD_Timer
#include "RBD_Timer.h"

#include "leds.h"

#define BUTTON1_PIN A2
#define INVERT true

#define FADER_1_PIN 0
#define FADER_2_PIN 1
#define SLEEP true
// Maximum value for the faders
#define MAX_FADER_VAL 1022

Leds *balloonLeds;

// init control button
OneButton button1(BUTTON1_PIN, INVERT);

// init faders
ResponsiveAnalogRead fader1(FADER_1_PIN, SLEEP);
ResponsiveAnalogRead fader2(FADER_2_PIN, SLEEP);

void handleButton1Click() {
  balloonLeds->incrementPreset();
}

void setup()
{
  balloonLeds = new Leds();
  //Serial.begin(9600);
  // handle buttons
  button1.attachClick(handleButton1Click);
  // set initial fader value
  balloonLeds->setFader1(map(fader1.getValue(),0,MAX_FADER_VAL,0,255));
  balloonLeds->setFader2(map(fader2.getValue(),0,MAX_FADER_VAL,0,255));
}

void loop()
{
  // handle button
  button1.tick();
  // update the ResponsiveAnalogRead objects (faders)
  fader1.update();
  fader2.update();
  if(fader1.hasChanged()) {
    balloonLeds->setFader1(map(fader1.getValue(),0,MAX_FADER_VAL,0,255));
  }
   if(fader2.hasChanged()) {
    balloonLeds->setFader2(map(fader2.getValue(),0,MAX_FADER_VAL,0,255));
  }
  // handle leds
  balloonLeds->loop();
}
