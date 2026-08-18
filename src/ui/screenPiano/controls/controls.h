// piano-ui.h
// Barra de botones de la interfaz
// Created by Martin on 2024-06-15.

#pragma once

#include <Arduino.h>
#include <Arduino_GFX_Library.h>       
#include "../../IUIComponent.h"
#include "../UIConfig.h"

using namespace UIConfig;

// Top bar UI controls for record/play/octave buttons and display

class ControlsUI : public IUIComponent {
public:
    int currentOctave = 4;
    bool isRecording = false;
    bool isPlaying = false;

    // Rect struct to define button areas
    struct Rect {
        int x, y, w, h;
    };

    Rect rectRec = {10, BAR_Y, 130, BTN_H};
    Rect rectPlay = {150, BAR_Y, 130, BTN_H};
    Rect rectOctDn = {300, BAR_Y, 36, BTN_H};
    Rect rectOctUp = {500, BAR_Y, 36, BTN_H};

    void draw(Arduino_RGB_Display* gfx);
    uint8_t handleTouch(int tx, int ty);

private:
    bool contains(const Rect& r, int x, int y);
    void drawPixelButton(Arduino_RGB_Display* gfx,
                         const Rect& r,
                         const char* label,
                         uint16_t faceColor,
                         uint16_t shadowColor);
    void drawOctaveDisplay(Arduino_RGB_Display* gfx);
    void toggleRec();
    void togglePlay();
    void changeOctave(int dir);
};