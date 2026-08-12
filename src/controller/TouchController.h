#pragma once

#include <Arduino.h>
#include <TAMC_GT911.h>
#include "UsbMidi.h"
#include "../ui/keyboard/PianoKeyboard.h"
#include "../ui/controls/Controls.h"
#include "../config/Config.h"


class TouchController {
public:
    TouchController(TAMC_GT911* touchSensor, ControlsUI* controlsUi, PianoKeyboard* keyboard, UsbMidi* midi);

    uint8_t update();

private:
    static int mapTouchX(int tx);
    static int mapTouchY(int ty);
    uint8_t handleScreenTouch(int tx, int ty);

    TAMC_GT911* touchSensor;
    ControlsUI* controlsUi;
    PianoKeyboard* keyboard;
    UsbMidi* midi;
};
