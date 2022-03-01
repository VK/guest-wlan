#ifndef __BUTTON_H
#define __BUTTON_H
#include "Arduino.h"
#include "wlan.h"

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

struct MyButton
{
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

volatile MyButton button_up = {37, 0, false};
volatile MyButton button_center = {38, 0, false};
volatile MyButton button_down = {39, 0, false};

void _button_handler(volatile MyButton &b)
{
    portENTER_CRITICAL(&mux);
    int state = digitalRead(b.PIN);
    if (state == LOW)
    {
        b.numberKeyPresses += 1;
        b.pressed = true;
    }
    portEXIT_CRITICAL(&mux);
}

void IRAM_ATTR button_up_handler()
{
    _button_handler(button_up);
}

void IRAM_ATTR button_center_handler()
{
    _button_handler(button_center);
}

void IRAM_ATTR button_down_handler()
{
    _button_handler(button_down);
}

void button_setup()
{
    pinMode(button_up.PIN, INPUT_PULLUP);
    attachInterrupt(button_up.PIN, button_up_handler, FALLING);

    pinMode(button_down.PIN, INPUT_PULLUP);
    attachInterrupt(button_down.PIN, button_down_handler, FALLING);

    pinMode(button_center.PIN, INPUT_PULLUP);
    attachInterrupt(button_center.PIN, button_center_handler, FALLING);
}

#endif