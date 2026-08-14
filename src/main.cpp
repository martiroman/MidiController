#include <Arduino.h>
#include "shared/hardware.h"
#include "controller/UIController.h"
#include "controller/TouchController.h"
#include "controller/MidiController.h"

// Global objects
MidiController midiController;
UIController uiController;
TouchController touchController;

void setup() {
    Serial.begin(115200);
    init_wifi_ota(); //begin() is called inside init_wifi_ota()
    Wire.begin(8, 9);
    
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
