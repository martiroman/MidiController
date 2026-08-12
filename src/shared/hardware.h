#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <usb/usb_host.h>
#include "../config/Config.h"

inline void ch422g_write(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CH422G_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Init panel RGB de la Waveshare 4.3"
inline Arduino_RGB_Display* init_display() {
    auto *bus = new Arduino_ESP32RGBPanel(
        5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
        14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
    );
    auto *gfx = new Arduino_RGB_Display(800, 480, bus);
    gfx->begin();
    gfx->fillScreen(BLACK);
    return gfx;
}

inline void init_wifi_ota() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("thecooders", "apuki2018");
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    ArduinoOTA.setHostname("microkorg-esp32");
    ArduinoOTA.begin();
}

inline void debug_screen(Arduino_RGB_Display* gfx, const char* msg, uint16_t color = WHITE) {
    static int line = 0;
    if (line > 15) { gfx->fillScreen(BLACK); line = 0; }
    gfx->setCursor(10, 10 + (line * 25));
    gfx->setTextColor(color);
    gfx->setTextSize(2);
    gfx->println(msg);
    line++;
    Serial.println(msg);
}
