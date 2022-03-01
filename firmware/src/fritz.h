#ifndef __FRITZ_H
#define __FRITZ_H
#include <tr064.h>
#include "display.h"
#include "random.h"

// The username if you created an account, "admin" otherwise
const char *fritz_user = "???";

// The password for the aforementioned account.
const char *fritz_pass = "???";

// IP address of your router. This should be "192.168.179.1" for most FRITZ!Boxes
const char *fritz_IP = "???";

// Port of the API of your router. This should be 49000 for all TR-064 devices.
const int PORT = 49000;

// TR-064 connection
TR064 *connection;
bool _fritz_enabled = false;

void fritz_setup()
{
    connection = new TR064(PORT, fritz_IP, fritz_user, fritz_pass);
    connection->init();
    _fritz_enabled = true;
}

void fritz_check()
{
    if (!_fritz_enabled)
    {
        display_update("Connect Fritz", false);
    }
    while (!_fritz_enabled)
    {
        delay(500);
        Serial.print(".");
    }
}

String fritz_targetSSID = "";
String fritz_gastSSID = "";
String fritz_gastPass = "";

int fritz_guestWlanStatus()
{
    String params[][2] = {{}};
    String req[][2] = {
        {"NewEnable", ""},
        {"NewSSID", ""},
    };
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "GetInfo", params, 0, req, 2);

    fritz_gastSSID = req[1][1];
    return (req[0][1]).toInt();
}

void fritz_guestWlanOn()
{
    String params[][2] = {{"NewEnable", "1"}};
    String req[][2] = {{}};
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "SetEnable", params, 1, req, 0);
}

void fritz_guestWlanOff()
{
    String params[][2] = {{"NewEnable", "0"}};
    String req[][2] = {{}};
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "SetEnable", params, 1, req, 0);
}

void fritz_setGuestWlanPass(String pass)
{
    Serial.print("New passwd: ");
    Serial.println(pass);

    String paramsA[][2] = {{}};
    String reqA[][2] = {
        {"NewWEPKey0", ""},
        {"NewWEPKey1", ""},
        {"NewWEPKey2", ""},
        {"NewWEPKey3", ""},
        {"NewPreSharedKey", ""},
        {"NewKeyPassphrase", ""},
    };
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "GetSecurityKeys", paramsA, 0, reqA, 6);

    reqA[5][1] = pass;
    String reqB[][2] = {{}};
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "SetSecurityKeys", reqA, 6, reqB, 0);
}

void fritz_setGuestWlanSSID(String ssid)
{
    Serial.print("New ssid: ");
    Serial.println(ssid);
    String params[][2] = {{"NewSSID", ssid}};
    String req[][2] = {{}};
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "SetSSID", params, 1, req, 0);
}

void fritz_getGuestWlanPass()
{
    String params[][2] = {{}};
    String req[][2] = {{"NewKeyPassphrase", ""}};
    connection->action("urn:dslforum-org:service:WLANConfiguration:3", "GetSecurityKeys", params, 0, req, 1);

    fritz_gastPass = req[0][1];
}

void fritz_updateStatus()
{
    fritz_getGuestWlanPass();
    Serial.println(fritz_gastSSID);
    Serial.println(fritz_gastPass);
    display_wlan(fritz_gastSSID, fritz_gastPass);
}

void fritz_enable()
{
    int guestOn = fritz_guestWlanStatus();
    if (Serial)
        Serial.printf("State of Guest WLAN: %d\n", guestOn);

    if (guestOn == 0)
    {
        display_update("Update code");

        String newPass = random_String(40);
        fritz_setGuestWlanPass(newPass);
        fritz_setGuestWlanSSID(fritz_targetSSID);
        fritz_guestWlanOn();

        bool newActive = false;

        while (!newActive)
        {
            guestOn = fritz_guestWlanStatus();
            fritz_getGuestWlanPass();

            Serial.print(guestOn);
            Serial.print("  ");
            Serial.println(fritz_gastPass);
            newActive = (guestOn == 1) && (fritz_gastPass == newPass);
        }

        fritz_updateStatus();
    }
    else
    {
        Serial.println("Use active guest wlan");
        fritz_updateStatus();
    }
}

#endif