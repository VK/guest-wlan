#ifndef __BATT_H
#define __BATT_H

#include "Arduino.h"

// display bat value
int batt_value = 100;

void batt_setup()
{
    batt_value = max(20, min(100, analogRead(35) - 805));
}

#endif