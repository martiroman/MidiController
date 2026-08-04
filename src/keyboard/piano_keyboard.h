// keyboard.h
// Piano keyboard logic and touch-to-note mapping
// Created by Martin on 2024-06-15.

#pragma once
#include "piano_config.h"

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define COLOR_BACKGROUND  0x0000
#define COLOR_WHITE_KEY   0xFFFF
#define COLOR_BLACK_KEY   0x10A2
#define COLOR_BORDER      0x0000

class PianoKeyboard {
public:
    static const int BLACK_KEY_INDICES[PianoConfig::BLACK_KEY_COUNT];

    int lastNotePlayed = -1;

    PianoKeyboard();
    void draw(Arduino_RGB_Display* gfx);

    // Returns the MIDI note index (0–12), or -1 if no key was hit.
    int getNoteAtTouch(int tx, int ty);

private:
    bool isBlackKey(int index) const;
    void drawWhiteKeys(Arduino_RGB_Display* gfx);
    void drawBlackKeys(Arduino_RGB_Display* gfx);
    int hitBlackKey(int tx) const;
    int hitWhiteKey(int tx) const;
    void drawControlPanel(Arduino_RGB_Display* gfx);
    void drawButton(Arduino_RGB_Display* gfx, int x, int y, int w, int h, const char* label);
};