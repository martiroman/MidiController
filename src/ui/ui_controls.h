// piano-ui.h
// Barra de botones de la interfaz
// Created by Martin on 2024-06-15.

#pragma once

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "../keyboard/piano_config.h"       

// Top bar UI controls for record/play/octave

#define COLOR_BTN_RED_FACE    0xE104   // Rojo Mario
#define COLOR_BTN_RED_SHADOW  0x8800
#define COLOR_BTN_GREEN_FACE  0x0D80
#define COLOR_BTN_GREEN_SHADOW 0x04A0
#define COLOR_BTN_BLACK_FACE   0x0000
#define COLOR_BTN_BLACK_SHADOW 0x0000
#define COLOR_BTN_BLUE_FACE   0x14BF
#define COLOR_BTN_BLUE_SHADOW 0x0A5E
#define COLOR_BTN_TEXT        0xFFFF
#define COLOR_DISPLAY_BG      0x0000
#define COLOR_DISPLAY_TEXT    0xFFE0 // Amarillo
#define COLOR_BAR_BG          0x0000
#define COLOR_BTN_SHADOW      0x0000

class ControlsUI {
public:
    static const int BAR_HEIGHT = 110;  // = KEY_Y_OFFSET
    static const int BTN_H = 62; // Altura de los botones
    static const int BTN_SHADOW = 8; // Sombra de los botones
    static const int BAR_Y = 16;    // Padding vertical en la barra

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
    bool handleTouch(int tx, int ty);

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