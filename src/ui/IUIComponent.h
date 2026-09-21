#pragma once
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "Colors.h"

class IUIComponent {
public:
    virtual ~IUIComponent() = default;

    virtual void draw(Arduino_RGB_Display* gfx) = 0;
    virtual uint8_t handleTouch(int tx, int ty) = 0;

};