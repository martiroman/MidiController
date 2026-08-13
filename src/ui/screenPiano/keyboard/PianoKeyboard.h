// keyboard.h
// Piano keyboard logic and touch-to-note mapping
// Created by Martin on 2024-06-15.

#pragma once
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "../../IUIComponent.h"

class PianoKeyboard : public IUIComponent {
public:
    int lastNotePlayed = -1;

    PianoKeyboard();
    void draw(Arduino_RGB_Display* gfx);
    uint8_t handleTouch(int tx, int ty);

private:
    bool isBlackKey(int index) const;
    void drawWhiteKeys(Arduino_RGB_Display* gfx);
    void drawBlackKeys(Arduino_RGB_Display* gfx);
    int hitBlackKey(int tx) const;
    int hitWhiteKey(int tx) const;
    void drawControlPanel(Arduino_RGB_Display* gfx);
    void drawButton(Arduino_RGB_Display* gfx, int x, int y, int w, int h, const char* label);
};