#pragma once

#include <Arduino.h>
#include <TAMC_GT911.h>
#include "UsbMidi.h"

class UIController;   // se resuelve en el .cpp, evita dependencia circular
class MidiController;

class TouchController {
public:
    TouchController();
    uint8_t update();
    void begin();
    void setUIController(UIController* controller);
    void setMidiController(MidiController* controller);

private:
    int w = 0;
    int h = 0;

    int mapTouchX(int);
    int mapTouchY(int);

    TAMC_GT911 touchSensor;
    UIController* ui = nullptr;
    MidiController* midi = nullptr;
    int activeNote = -1;
};
