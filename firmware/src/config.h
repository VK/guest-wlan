#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "wlan.h"
#include "fritz.h"

void config_setup()
{

    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    File file = SPIFFS.open("/config.json", FILE_READ);
    if (!file)
    {
        Serial.println("No configuration stored");
        return;
    }
    else
    {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, file.readString());

        wlan_ssid = (new String(doc["wlan"]["ssid"].as<String>()))->c_str();
        wlan_password = (new String(doc["wlan"]["password"].as<String>()))->c_str();

        fritz_user = (new String(doc["fritz"]["user"].as<String>()))->c_str();
        fritz_pass = (new String(doc["fritz"]["password"].as<String>()))->c_str();
        fritz_IP = (new String(doc["fritz"]["IP"].as<String>()))->c_str();
        fritz_targetSSID = (new String(doc["fritz"]["ssid"].as<String>()))->c_str();
        }
}

#endif