# Fritz Guest WLAN
<img align="right" src=".media/example.gif">

Gadget to activate and deactivate the **Guest WLAN** of an AMV **FRITZ!Box**.

If the WLAN is switched on, the QR code is automatically generated and shown permanently on the E-ink display.
Each time a new random key is generated. The microcontroller is disabled most of the time, allowing for years of battery life.


## Ingredients:

* **Microcontroller** with E-ink display [M5Stack Core Ink](https://shop.m5stack.com/collections/m5-controllers/products/m5stack-esp32-core-ink-development-kit1-54-elnk-display)

* **Fritz!Box API** https://github.com/Aypac/Arduino-TR-064-SOAP-Library

* **Create QR** https://github.com/ricmoo/qrcode
