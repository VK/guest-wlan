#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>
#include <GxEPD2_BW.h>
//#include <EPD/GxEPD2_154_M09.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "qrcode.h"
#include <U8g2_for_Adafruit_GFX.h>
#include "SPIFFS.h"
#include "batt.h"

// GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67 ::HEIGHT> display(GxEPD2_154_D67(/*CS=*/26, /*DC=*/25, /*RST=*/33, /*BUSY=*/27)); // GDEP015OC1 no longer available
GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09 ::HEIGHT> display(GxEPD2_154_M09(/*CS=*/9, /*DC=*/15, /*RST=*/0, /*BUSY=*/4));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

// current qr code
String display_code = "";

// 'wlan', 37x29px  (http://javl.github.io/image2cpp/)
const unsigned char epd_bitmap_wlan[] PROGMEM = {
    0x00, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x07,
    0xf8, 0x00, 0xff, 0x00, 0x1f, 0xc0, 0x00, 0x1f, 0xc0, 0x3f, 0x00, 0x00, 0x07, 0xe0, 0x7c, 0x00,
    0x00, 0x01, 0xf0, 0xf8, 0x01, 0xfc, 0x00, 0xf8, 0xe0, 0x0f, 0xff, 0x80, 0x38, 0x40, 0x7f, 0xff,
    0xf0, 0x10, 0x00, 0xff, 0x8f, 0xf8, 0x00, 0x03, 0xf8, 0x00, 0xfe, 0x00, 0x07, 0xe0, 0x00, 0x3f,
    0x00, 0x07, 0x80, 0x00, 0x0f, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00,
    0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0x07, 0xe0, 0x00, 0x00,
    0x1c, 0x01, 0xc0, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xfc,
    0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00,
    0x00};

// 'button' 33x80px
const unsigned char epd_bitmap_button_init[] PROGMEM =
    {
        0x01, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x01, 0x80, 0x00, 0x03, 0xc0, 0x07, 0x00, 0x00,
        0x0f, 0x80, 0x0f, 0x00, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x00, 0xf8, 0x00, 0x7f, 0x00, 0x01, 0xe0,
        0x00, 0xff, 0x00, 0x01, 0xff, 0xc3, 0xfe, 0x00, 0x01, 0xff, 0xc7, 0xfe, 0x00, 0x00, 0x00, 0x01,
        0xfe, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7e, 0x00, 0xfc, 0x00, 0x00, 0xff, 0x80, 0xec,
        0x00, 0x01, 0xc1, 0xc1, 0xcc, 0x00, 0x01, 0x80, 0xc1, 0xc0, 0x00, 0x03, 0x00, 0xc3, 0x80, 0x00,
        0x03, 0x00, 0x43, 0x80, 0x00, 0x03, 0x00, 0xc3, 0x00, 0x00, 0x01, 0x80, 0xc7, 0x00, 0x00, 0x01,
        0xe3, 0xc6, 0x00, 0x00, 0x00, 0xff, 0x86, 0x00, 0x00, 0x00, 0x3e, 0x0e, 0x00, 0x00, 0x00, 0x00,
        0x0c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c,
        0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
        0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
        0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60,
        0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
        0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x03,
        0x18, 0x38, 0x00, 0x00, 0x03, 0x18, 0x38, 0x00, 0x00, 0x03, 0x18, 0x18, 0x00, 0x00, 0x03, 0x18,
        0x18, 0x00, 0x00, 0x03, 0x18, 0x1c, 0x00, 0x00, 0x03, 0xff, 0xcc, 0x00, 0x00, 0x01, 0xff, 0xcc,
        0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x00, 0x03, 0x18, 0x06, 0x00,
        0x00, 0x03, 0x18, 0x07, 0x00, 0x00, 0x03, 0x18, 0x03, 0x00, 0x00, 0x03, 0x18, 0x03, 0x80, 0x00,
        0x03, 0xff, 0xc3, 0x80, 0x00, 0x01, 0xff, 0xc1, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xcc, 0x00, 0x00,
        0x00, 0x00, 0xfc, 0x00, 0x00, 0x1c, 0x00, 0xfc, 0x00, 0x00, 0xff, 0x00, 0x7c, 0x00, 0x01, 0xff,
        0x83, 0xfe, 0x00, 0x01, 0x80, 0xc7, 0xfe, 0x00, 0x03, 0x00, 0xc1, 0xfe, 0x00, 0x03, 0x00, 0xc0,
        0xff, 0x00, 0x03, 0x00, 0xc0, 0x7f, 0x00, 0x01, 0x80, 0xc0, 0x1f, 0x00, 0x01, 0xc1, 0xc0, 0x0f,
        0x00, 0x00, 0xff, 0x80, 0x03, 0x80, 0x00, 0x7f, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char epd_bitmap_button_start[] PROGMEM =
    {
        0x01, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x01, 0x80, 0x00, 0x03, 0xc0, 0x07, 0x00, 0x00,
        0x0f, 0x80, 0x0f, 0x00, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x00, 0xf8, 0x00, 0x7f, 0x00, 0x01, 0xe0,
        0x00, 0xff, 0x00, 0x01, 0xff, 0xc3, 0xfe, 0x00, 0x01, 0xff, 0xc7, 0xfe, 0x00, 0x00, 0x00, 0x01,
        0xfe, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7e, 0x00, 0xfc, 0x00, 0x00, 0xff, 0x80, 0xec,
        0x00, 0x01, 0xc1, 0xc1, 0xcc, 0x00, 0x01, 0x80, 0xc1, 0xc0, 0x00, 0x03, 0x00, 0xc3, 0x80, 0x00,
        0x03, 0x00, 0x43, 0x80, 0x00, 0x03, 0x00, 0xc3, 0x00, 0x00, 0x01, 0x80, 0xc7, 0x00, 0x00, 0x01,
        0xe3, 0xc6, 0x00, 0x00, 0x00, 0xff, 0x86, 0x00, 0x00, 0x00, 0x3e, 0x0e, 0x00, 0x00, 0x00, 0x00,
        0x0c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c,
        0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00,
        0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
        0x18, 0x70, 0x00, 0x00, 0x00, 0xf8, 0x60, 0x00, 0x00, 0x07, 0xf0, 0x60, 0x00, 0x00, 0x3f, 0xf0,
        0x60, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xf0, 0x60,
        0x00, 0x00, 0x03, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x78, 0x60, 0x00, 0x00, 0x00, 0x08, 0x70, 0x00,
        0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x03,
        0x18, 0x38, 0x00, 0x00, 0x03, 0x18, 0x38, 0x00, 0x00, 0x03, 0x18, 0x18, 0x00, 0x00, 0x03, 0x18,
        0x18, 0x00, 0x00, 0x03, 0x18, 0x1c, 0x00, 0x00, 0x03, 0xff, 0xcc, 0x00, 0x00, 0x01, 0xff, 0xcc,
        0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x00, 0x03, 0x18, 0x06, 0x00,
        0x00, 0x03, 0x18, 0x07, 0x00, 0x00, 0x03, 0x18, 0x03, 0x00, 0x00, 0x03, 0x18, 0x03, 0x80, 0x00,
        0x03, 0xff, 0xc3, 0x80, 0x00, 0x01, 0xff, 0xc1, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xcc, 0x00, 0x00,
        0x00, 0x00, 0xfc, 0x00, 0x00, 0x1c, 0x00, 0xfc, 0x00, 0x00, 0xff, 0x00, 0x7c, 0x00, 0x01, 0xff,
        0x83, 0xfe, 0x00, 0x01, 0x80, 0xc7, 0xfe, 0x00, 0x03, 0x00, 0xc1, 0xfe, 0x00, 0x03, 0x00, 0xc0,
        0xff, 0x00, 0x03, 0x00, 0xc0, 0x7f, 0x00, 0x01, 0x80, 0xc0, 0x1f, 0x00, 0x01, 0xc1, 0xc0, 0x0f,
        0x00, 0x00, 0xff, 0x80, 0x03, 0x80, 0x00, 0x7f, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'power', 25x15px
const unsigned char epd_bitmap_power[] PROGMEM = {
    0x7c, 0x00, 0x00, 0x00, 0xff, 0x80, 0x80, 0x00, 0x7f, 0xc1, 0x80, 0x00, 0x01, 0xf1, 0xc0, 0x00,
    0x00, 0x79, 0xe0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x07, 0xf8, 0x00,
    0x00, 0x0f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x0f, 0xfe, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x01, 0x80};

void _display_draw_wlan(bool off = false)
{

    display.fillCircle(100, 100, 29, GxEPD_BLACK);
    display.fillCircle(100, 100, 26, GxEPD_WHITE);
    display.drawBitmap(82, 87, epd_bitmap_wlan, 37, 29, 0);

    if (off)
    {

        display.drawLine(84, 80, 120, 116, GxEPD_WHITE);
        display.drawLine(83, 80, 119, 116, GxEPD_WHITE);
        display.drawLine(82, 80, 118, 116, GxEPD_WHITE);

        display.drawLine(81, 80, 121, 120, GxEPD_BLACK);
        display.drawLine(80, 80, 120, 120, GxEPD_BLACK);
        display.drawLine(79, 80, 119, 120, GxEPD_BLACK);
        display.drawLine(78, 80, 118, 120, GxEPD_BLACK);

        display.drawLine(81, 84, 117, 120, GxEPD_WHITE);
        display.drawLine(80, 84, 116, 120, GxEPD_WHITE);
        display.drawLine(79, 84, 115, 120, GxEPD_WHITE);
    }
}

void display_update(String info = "", bool full = false, bool button = false, bool start = false, bool power = false)
{

    int16_t tw = u8g2Fonts.getUTF8Width(info.c_str()); // text box width
    int16_t ta = u8g2Fonts.getFontAscent();            // positive
    int16_t td = u8g2Fonts.getFontDescent();           // negative; in mathematicians view
    int16_t th = ta - td;                              // text box height
    uint16_t x = (display.width() - tw) / 2;
    uint16_t y = (display.height() - th) / 2 + ta + 50;
    display_code = "";

    if (full)
    {
        display.setFullWindow();
        display.firstPage();
    }
    else
    {
        display.setPartialWindow(0, 0, display.width(), display.height());
        display.firstPage();
    }
    do
    {

        display.fillScreen(GxEPD_WHITE);
        _display_draw_wlan(info == "OFF");
        if (button)
        {
            if (start)
            {
                display.drawBitmap(165, 21, epd_bitmap_button_start, 33, 80, GxEPD_BLACK);
            }
            else
            {
                display.drawBitmap(165, 21, epd_bitmap_button_init, 33, 80, GxEPD_BLACK);
            }
        }

        display.fillRect(display.width() - 35, 5, 30, 11, GxEPD_BLACK);
        display.fillRect(display.width() - 38, 8, 3, 5, GxEPD_BLACK);
        display.fillRect(display.width() - 33, 7, (100 - batt_value) / 3.4, 7, GxEPD_WHITE);

        if (info.length() > 0)
        {
            u8g2Fonts.setCursor(x, y);
            u8g2Fonts.setFontDirection(0);
            u8g2Fonts.print(info.c_str());
        }

        if (start)
        {
            u8g2Fonts.setCursor(163, 91);
            u8g2Fonts.setFontDirection(3);
            u8g2Fonts.print("START");
        }

        if (power)
        {
            u8g2Fonts.setCursor(100, 192);
            u8g2Fonts.setFontDirection(0);
            u8g2Fonts.print("POWER");
            display.drawBitmap(175, 184, epd_bitmap_power, 25, 15, 0);
        }

    } while (display.nextPage());
}

void display_wlan(String ssid, String pass)
{
    // The structure to manage the QR code
    QRCode qrcode;

    // create the wlan qr key
    String code = "WIFI:T:WPA;S:" + ssid + ";P:" + pass + ";;";

    if (code == display_code)
        return;

    display_code = code;

    int qr_vers = 5;
    if (code.length() > 40)
    {
        qr_vers = 8;
    }

    // Allocate a chunk of memory to store the QR code
    uint8_t qrcodeBytes[qrcode_getBufferSize(qr_vers)];

    qrcode_initText(&qrcode, qrcodeBytes, qr_vers, ECC_HIGH, code.c_str());

    // clear the display first
    display.setFullWindow();
    display.firstPage();

    int boxsize = (int)floor(196.0 / (float)(qrcode.size));
    int offset = (200 - boxsize * qrcode.size) / 2;

    do
    {
        for (uint8_t y = 0; y < qrcode.size; y++)
        {
            for (uint8_t x = 0; x < qrcode.size; x++)
            {
                if (qrcode_getModule(&qrcode, x, y))
                {

                    display.fillRect(
                        offset + (int)(x * boxsize),
                        offset + (int)(y * boxsize),
                        boxsize,
                        boxsize,
                        GxEPD_BLACK);
                }
            }
        }
        _display_draw_wlan();
    } while (display.nextPage());

    display.setFullWindow();
}

void display_setup()
{

    display.init(115200);
    u8g2Fonts.begin(display);
    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setFontDirection(0);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvR14_tf);
}

void display_loop()
{
}

#endif