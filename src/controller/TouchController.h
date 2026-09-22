#pragma once

#include <Arduino.h>
#include <TAMC_GT911.h>
#include "UsbMidi.h"

class TouchController {
public:
    TouchController();
    void update();
    void begin();
    void getTouchCoordinates(int&, int&);
    bool isTouchActive();
    
private:
    int w = 0;
    int h = 0;

    int x = 0;
    int y = 0;
    bool isTouched = false;

    TAMC_GT911 touchSensor;
};
