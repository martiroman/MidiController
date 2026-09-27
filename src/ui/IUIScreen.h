#pragma once

#include <Arduino_GFX_Library.h>
#include "UIEvent.h"

class IUIScreen {
public:
    int width;
    int height;
    Arduino_RGB_Display* gfx;

    virtual ~IUIScreen() = default;

    virtual void draw() = 0;
    virtual UIEvent handleTouch(int tx, int ty) = 0;
};