#include <Arduino.h>
#include "shared/hardware.h"
#include "UsbMidi.h"
#include "ui/keyboard/PianoKeyboard.h"
#include "ui/controls/Controls.h"
#include "ui/UI_Intro.h"
#include "controller/TouchController.h"

// Global objects
Arduino_RGB_Display* gfx;
TAMC_GT911 ts(8, 9, 4, -1, 800, 480);
UsbMidi midi;

PianoKeyboard keyboard;
ControlsUI ui;
TouchController touchController(&ts, &ui, &keyboard, &midi);

void setup() {
    Serial.begin(115200);
    init_wifi_ota();
    gfx = init_display();
    ts.begin();
    Wire.begin(8, 9);
    
    midi.begin();
    debug_screen(gfx, "USB MIDI Inicializado", WHITE);

    String ipStr = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";
    
    debug_screen(gfx, ipStr.c_str(), WHITE);
    
    delay(5000);
    intro(midi, gfx);
    keyboard.draw(gfx);
    ui.draw(gfx);
}

void loop() {
    ArduinoOTA.handle();
    midi.update();
    touchController.update();
    delay(10); // Estabilidad para los rebotes del touch
}
