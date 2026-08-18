#include <Arduino.h>
#include "shared/CH422G.h"
#include "shared/WifiOTA.h"
#include "controller/UIController.h"
#include "controller/TouchController.h"
#include "controller/MidiController.h"

// Global objects
MidiController midiController;
UIController uiController;
TouchController touchController;

void setup() {
    //Serial.begin(115200);
    initWifiOTA();
    initWire();

    midiController.begin();
    touchController.begin();
    uiController.begin();

    String ipStr = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";    
    uiController.debugMsg(ipStr.c_str(), WHITE);
    
    delay(2000);
    
    uiController.drawCurrentScreen();
}

void loop() {
    ArduinoOTA.handle();
    midiController.update();
    touchController.update();
    
    delay(10); // Estabilidad para los rebotes del touch
}
