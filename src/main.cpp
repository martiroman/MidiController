#include <Arduino.h>
#include "shared/CH422G.h"
#include "shared/WifiOTA.h"
#include "controller/AppController.h"
#include "controller/UIController.h"
#include "controller/TouchController.h"
#include "controller/MidiController.h"

// Global objects
AppController appController;

void setup() {
    Serial.begin(115200);

    Serial.println("=== Iniciando WiFi ===");
    String wifiStatus = initWifiOTA();
    Serial.println(wifiStatus.c_str());
    Serial.flush();

    Serial.println("=== Iniciando Wire ===");
    initWire();
    Serial.println("Wire OK");
    Serial.flush();

    appController.begin();
}

void loop() {
    ArduinoOTA.handle();
    appController.update();

    delay(10); // Estabilidad para los rebotes del touch
}
