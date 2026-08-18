#pragma once

#include <Arduino.h>
#include <TAMC_GT911.h>
#include "UsbMidi.h"
#include "../ui/IUIScreen.h"

class TouchController {
public:
    TouchController();    
    uint8_t update();
    void begin();

private:
    int w = 0;
    int h = 0;

    int mapTouchX(int);
    int mapTouchY(int);
    uint8_t handleScreenTouch(int, int);

    TAMC_GT911 touchSensor;
    IUIScreen* screen;
};
