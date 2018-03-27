/*
	A LED Contnroller using Arduino for WS2811 LED's.
  Used for a balloon shaped lamp I will build with my nephew.

  Refer to github repo for more information.

  Created 11/2017 by Hendrik Putzek

*/

// https://github.com/mathertel/OneButton/
#include "OneButton.h"

// https://github.com/rocketscream/Low-Power/
#include "LowPower.h"

// https://github.com/dxinteractive/ResponsiveAnalogRead
#include "ResponsiveAnalogRead.h"

// https://github.com/alextaujenis/RBD_Timer
#include "RBD_Timer.h"

// https://github.com/rlogiacco/BatterySense/
#include <Battery.h>

#include "leds.h"

#define BUTTON1_PIN 2
#define INVERT true

#define FADER_1_PIN A0
#define FADER_2_PIN A1

#define BATTERY_SENSE_PIN A2

#define SLEEP true
// Maximum value for the faders
#define MAX_FADER_VAL 1022

Leds *balloonLeds;

// init control button
OneButton button1(BUTTON1_PIN, INVERT);

// init faders
ResponsiveAnalogRead fader1(FADER_1_PIN, SLEEP);
ResponsiveAnalogRead fader2(FADER_2_PIN, SLEEP);

// sleep timer
#define SLEEP_10_MINUTES 600000
RBD::Timer sleepTimer(SLEEP_10_MINUTES);

// battery status
Battery battery(2550, 3700, BATTERY_SENSE_PIN);

void handleButton1Click()
{
  balloonLeds->incrementPreset();
}

void handleButton1Hold()
{
  balloonLeds->blinkSuccess();
  sleepTimer.restart();
}

void handleButton1DoubleClick()
{
  balloonLeds->saveCurrentPresetAsDefault();
  balloonLeds->blinkSuccess();
};

void wakeUp()
{
  detachInterrupt(digitalPinToInterrupt(BUTTON1_PIN));
  balloonLeds->blinkSuccess();
  balloonLeds->setBrightness(balloonLeds->getSavedBrightness());
}

void goToSleep()
{
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), wakeUp, LOW);
  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void setup()
{
  balloonLeds = new Leds();
  // battery level
  battery.begin();
  balloonLeds->showBatteryLevel(battery.level());
  
  sleepTimer.stop();
  //Serial.begin(9600);
  // handle buttons
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  button1.attachClick(handleButton1Click);
  button1.attachDoubleClick(handleButton1DoubleClick);
  button1.attachLongPressStart(handleButton1Hold);
}

void loop()
{
  // handle button
  button1.tick();
  // update the ResponsiveAnalogRead objects (faders)
  fader1.update();
  fader2.update();
  if (fader1.hasChanged())
  {
    balloonLeds->setFader1(map(fader1.getValue(), 0, MAX_FADER_VAL, 0, 255));
  }
  if (fader2.hasChanged())
  {
    balloonLeds->setFader2(map(fader2.getValue(), 0, MAX_FADER_VAL, 0, 255));
  }

  if (sleepTimer.isActive())
  {
    balloonLeds->setBrightness(map(sleepTimer.getInversePercentValue(), 0, 100, 0, 120));
  }
  if (sleepTimer.onExpired())
  {
    balloonLeds->setBrightness(0);
    goToSleep();
  }
  // handle leds
  balloonLeds->loop();

  // shut down if battery level gets too low
  if(battery.level() <= 1) {
    balloonLeds->showBatteryLevel(battery.level());
    balloonLeds->showBatteryLevel(battery.level());
    goToSleep();
  }
}
