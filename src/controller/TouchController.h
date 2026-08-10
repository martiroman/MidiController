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

    void update();

private:
    static int mapTouchX(int tx);
    static int mapTouchY(int ty);
    void handleScreenTouch(int tx, int ty);
    void releaseCurrentNote();

    TAMC_GT911* touchSensor;
    ControlsUI* controlsUi;
    PianoKeyboard* keyboard;
    UsbMidi* midi;
    int lastNoteTouched = -1;
};
