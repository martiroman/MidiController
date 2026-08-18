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
    Serial.begin(115200);

    delay(10000);

    Serial.println("=== Iniciando WiFi ===");
    String wifiStatus = initWifiOTA();
    Serial.println(wifiStatus.c_str());
    Serial.flush();
    delay(10000);

    Serial.println("=== Iniciando Wire ===");
    initWire();
    Serial.println("Wire OK");
    Serial.flush();
    delay(10000);

    Serial.println("=== Iniciando UI ===");
    Serial.flush();
    uiController.begin();
    delay(5000);
    Serial.println("=== Iniciando MIDI ===");
    Serial.flush();
    midiController.begin();
    delay(5000);
    Serial.println("=== Iniciando Touch ===");
    Serial.flush();
    touchController.begin();

    uiController.debugMsg(wifiStatus.c_str(), WHITE);
    
    delay(2000);
    
    uiController.drawCurrentScreen();
}

void loop() {
    ArduinoOTA.handle();
    midiController.update();
    touchController.update();
    
    delay(10); // Estabilidad para los rebotes del touch
}
