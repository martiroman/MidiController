#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "../config/WifiCfg.h"

using namespace WifiConfig;

inline String initWifiOTA() {
    WiFi.mode(WIFI_STA);

    if (!WiFi.config(getLocalIP(), getGateway(), getSubnet(), getDNS())) {
        //TODO: Manejar error de configuracion IP estatica
    }

    WiFi.begin(SSID, PASS);
    
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
    }
    
    ArduinoOTA.setHostname(HOST);
    ArduinoOTA.begin();

    String statusRes = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";
    return statusRes;
}