#pragma once

#include <Arduino_GFX_Library.h>

class IUIScreen {
public:
    int width;
    int height;

    virtual ~IUIScreen() = default;

    virtual void draw(Arduino_RGB_Display* gfx) = 0;
    virtual uint8_t handleTouch(int tx, int ty) = 0;
};