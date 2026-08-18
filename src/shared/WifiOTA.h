#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "../config/WifiCfg.h"


inline void initWifiOTA() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WifiConfig::SSID, WifiConfig::PASS);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    ArduinoOTA.setHostname(WifiConfig::HOST);
    ArduinoOTA.begin();
}
