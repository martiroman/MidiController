#pragma once
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "../config/Colors.h"

class IUIComponent {
public:
    // Destructor virtual para asegurar una correcta limpieza de memoria
    virtual ~IUIComponent() = default;

    // Métodos puros (= 0): Toda clase hija OBLIGA a implementarlos
    virtual void draw(Arduino_RGB_Display* gfx) = 0;
    virtual uint8_t handleTouch(int tx, int ty) = 0;

};