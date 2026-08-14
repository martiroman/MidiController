#pragma once
#include <Arduino.h>
#include <Arduino_GFX_Library.h>

class IController {
public:
    virtual ~IController() = default;

    virtual void begin() = 0;
    virtual void update() = 0;

};