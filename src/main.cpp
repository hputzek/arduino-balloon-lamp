/*
	A LED Contnroller using Arduino for WS2811 LED's.
  Used for a balloon shaped lamp I will build with my nephew.

  Refer to github repo for more information.

  Created 11/2017 by Hendrik Putzek

*/


// https://github.com/mathertel/OneButton/
#include "OneButton.h";

// https://github.com/rocketscream/Low-Power/
#include "LowPower.h";

#include "leds.h"

Leds balloonLeds;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  balloonLeds.loop();
}
