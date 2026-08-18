#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "../config/WifiCfg.h"

using namespace WifiConfig;

inline String initWifiOTA() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);
    //while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    ArduinoOTA.setHostname(HOST);
    ArduinoOTA.begin();

    String statusRes = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";
    return statusRes;
}
