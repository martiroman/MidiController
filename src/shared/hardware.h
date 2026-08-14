#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <usb/usb_host.h>
#include "../config/Config.h"

inline void ch422g_write(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CH422G_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

inline void init_wifi_ota() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("thecooders", "apuki2018");
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    ArduinoOTA.setHostname("microkorg-esp32");
    ArduinoOTA.begin();
}
