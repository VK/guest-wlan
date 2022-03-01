#include "Arduino.h"
#include "M5CoreInk.h"
#include "config.h"
#include "button.h"
#include "wlan.h"
#include "display.h"
#include "batt.h"
#include "fritz.h"

#define POWER_HOLD_PIN 12
#define POWER_LED_PIN 10

void setup()
{

  pinMode(POWER_HOLD_PIN, OUTPUT);
  digitalWrite(POWER_HOLD_PIN, HIGH); // Hold power
  pinMode(POWER_LED_PIN, OUTPUT);

  Serial.begin(115200);
  config_setup();
  display_setup();
  batt_setup();

  if (batt_value < 25)
  {
    display_update("Battery low", false);
    M5.Speaker.tone(659, 50);

    delayMicroseconds(50);
    M5.shutdown();
    digitalWrite(POWER_HOLD_PIN, LOW);
  }

  display_update("", false, true, false);

  wlan_setup();

  M5.Speaker.begin();
  M5.Speaker.tone(880, 50);

  button_setup();
}

int state = 0;
unsigned long last_action = 0;

void loop()
{
  unsigned long now = micros();

  if (button_up.pressed)
  {
    button_up.pressed = false;
    if (state != 1)
    {
      state = 1;
      last_action = now;
      display_update("ON", false, true, true);
    }
  }

  if (button_down.pressed)
  {
    button_down.pressed = false;
    if (state != -1)
    {
      state = -1;
      last_action = now;
      display_update("OFF", false, true, true);
    }
  }

  if (button_center.pressed)
  {
    last_action = now;

    if (state == 1)
    {
      wlan_check();
      fritz_check();

      fritz_enable();
      M5.Speaker.tone(987, 50);
    }
    else if (state == -1)
    {
      wlan_check();
      fritz_check();

      fritz_guestWlanOff();
      display_update("Guest WLAN", true, false, false, true);
      M5.Speaker.tone(659, 50);
    }

    delayMicroseconds(25000);
    M5.Speaker.end();
    M5.shutdown();
    digitalWrite(POWER_HOLD_PIN, LOW);
  }

  if (now - last_action > 20000000)
  {
    display_update("Guest WLAN", true, false, false, true);
    M5.Speaker.tone(659, 50);
    M5.Speaker.update();
    delayMicroseconds(25000);
    M5.Speaker.end();
    M5.shutdown();
    digitalWrite(POWER_HOLD_PIN, LOW);
  }

  M5.Speaker.update();
}
