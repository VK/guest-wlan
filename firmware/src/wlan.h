#ifndef __WLAN_H
#define __WLAN_H

#include <WiFiGeneric.h>
#include <WiFi.h>

#include "button.h"
#include "fritz.h"

// Wlan credentials are set by config
const char *wlan_ssid = "???";
const char *wlan_password = "???";

void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    if (event == 7)
    {
        delay(200);
        fritz_setup();
    }
}

void wlan_setup()
{
    WiFi.disconnect(true);
    WiFi.onEvent(WiFiEvent);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    WiFi.begin(wlan_ssid, wlan_password);
}

void wlan_check()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        display_update("Connect WLAN", false);
    }
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

#endif
