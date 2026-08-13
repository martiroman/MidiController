#include <Arduino.h>
#include "shared/hardware.h"
#include "UsbMidi.h"
#include "controller/UIController.h"
#include "controller/TouchController.h"

// Global objects
UsbMidi midi;
UIController uiController;
TouchController touchController;

void setup() {
    Serial.begin(115200);
    init_wifi_ota(); //begin() is called inside init_wifi_ota()
    uiController.begin();
    Wire.begin(8, 9);
    midi.begin();
    touchController.begin();
    uiController.begin();

    //debug_screen(gfx, "USB MIDI Inicializado", WHITE);

    //String ipStr = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";    
    //debug_screen(gfx, ipStr.c_str(), WHITE);
    
    delay(2000);
    
    uiController.drawCurrentScreen();
}

void loop() {
    ArduinoOTA.handle();
    midi.update();
    touchController.update();
    
    delay(10); // Estabilidad para los rebotes del touch
}
