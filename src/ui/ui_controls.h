// piano-ui.h
// Barra de botones de la interfaz
// Created by Martin on 2024-06-15.

#pragma once

#include <Arduino.h>
// Top bar UI controls for record/play/octave

#define COLOR_BTN_RED_FACE    0xE104   // Rojo Mario
#define COLOR_BTN_RED_SHADOW  0x8800
#define COLOR_BTN_GREEN_FACE  0x0D80
#define COLOR_BTN_GREEN_SHADOW 0x05A0
#define COLOR_BTN_BLUE_FACE   0x14BF
#define COLOR_BTN_BLUE_SHADOW 0x0A5E
#define COLOR_BTN_TEXT        0xFFFF
#define COLOR_DISPLAY_BG      0x0000
#define COLOR_DISPLAY_TEXT    0xFFE0   // Amarillo
#define COLOR_BAR_BG          0x100D   // Azul oscuro
#define COLOR_BTN_SHADOW      0x0000

class PianoUI {
public:
    static const int BAR_HEIGHT    = 110;  // = KEY_Y_OFFSET
    static const int BTN_H         = 36;
    static const int BTN_SHADOW    = 4;
    static const int BAR_Y         = 8;    // Padding vertical en la barra

    int  currentOctave = 4;
    bool isRecording   = false;
    bool isPlaying     = false;

    // Rects de los botones (para hit-testing)
    struct Rect { int x, y, w, h; };
    Rect rectRec   = {10,  BAR_Y, 72,  BTN_H};
    Rect rectPlay  = {90,  BAR_Y, 80,  BTN_H};
    Rect rectOctDn = {190, BAR_Y, 36,  BTN_H};
    Rect rectOctUp = {300, BAR_Y, 36,  BTN_H};

    void draw(Arduino_RGB_Display* gfx) {
        // Fondo de la barra
        gfx->fillRect(0, 0, PianoConfig::SCREEN_WIDTH, BAR_HEIGHT, COLOR_BAR_BG);

        drawPixelButton(gfx, rectRec,
            isRecording ? "STOP" : "REC",
            isRecording ? COLOR_BTN_RED_FACE  : COLOR_BTN_RED_FACE,
            COLOR_BTN_RED_SHADOW);

        drawPixelButton(gfx, rectPlay,
            isPlaying ? "STOP" : "PLAY",
            isPlaying ? 0xFFE0 : COLOR_BTN_GREEN_FACE,
            isPlaying ? 0xA080 : COLOR_BTN_GREEN_SHADOW);

        drawPixelButton(gfx, rectOctDn, "<", COLOR_BTN_BLUE_FACE, COLOR_BTN_BLUE_SHADOW);
        drawPixelButton(gfx, rectOctUp, ">", COLOR_BTN_BLUE_FACE, COLOR_BTN_BLUE_SHADOW);

        drawOctaveDisplay(gfx);
    }

    // Devuelve true si toca boton
    bool handleTouch(int tx, int ty) {
        if (contains(rectRec, tx, ty))   { toggleRec();   return true; }
        if (contains(rectPlay, tx, ty))  { togglePlay();  return true; }
        if (contains(rectOctDn, tx, ty)) { changeOctave(-1); return true; }
        if (contains(rectOctUp, tx, ty)) { changeOctave(+1); return true; }
        return false;
    }

private:
    bool contains(const Rect& r, int x, int y) {
        return x >= r.x && x <= r.x + r.w &&
               y >= r.y && y <= r.y + r.h;
    }

    void drawPixelButton(Arduino_RGB_Display* gfx,
                         const Rect& r,
                         const char* label,
                         uint16_t faceColor,
                         uint16_t shadowColor)
    {
        // Sombra
        gfx->fillRect(r.x, r.y + r.h, r.w, BTN_SHADOW, shadowColor);

        // Cara del botón
        gfx->fillRect(r.x, r.y, r.w, r.h, faceColor);

        // Borde exterior (1px negro)
        gfx->drawRect(r.x, r.y, r.w, r.h + BTN_SHADOW, COLOR_BTN_SHADOW);

        // Texto centrado
        gfx->setTextColor(COLOR_BTN_TEXT);
        gfx->setTextSize(1);
        int textW = strlen(label) * 6;
        int textX = r.x + (r.w - textW) / 2;
        int textY = r.y + (r.h - 8)  / 2;
        gfx->setCursor(textX, textY);
        gfx->print(label);
    }

    void drawOctaveDisplay(Arduino_RGB_Display* gfx) {
        int x = rectOctDn.x + rectOctDn.w + 4;
        int w = rectOctUp.x - x - 4;

        // Caja negra con borde dorado
        gfx->fillRect(x, BAR_Y, w, BTN_H, COLOR_DISPLAY_BG);
        gfx->drawRect(x,     BAR_Y,     w,     BTN_H,     COLOR_DISPLAY_TEXT);
        gfx->drawRect(x + 1, BAR_Y + 1, w - 2, BTN_H - 2, COLOR_DISPLAY_TEXT);

        // Texto "OCT X"
        gfx->setTextColor(COLOR_DISPLAY_TEXT);
        gfx->setTextSize(1);
        char buf[8];
        snprintf(buf, sizeof(buf), "OCT %d", currentOctave);
        int textW = strlen(buf) * 6;
        gfx->setCursor(x + (w - textW) / 2, BAR_Y + (BTN_H - 8) / 2);
        gfx->print(buf);
    }

    void toggleRec()  { isRecording = !isRecording; }
    void togglePlay() { isPlaying   = !isPlaying; }
    void changeOctave(int dir) {
        currentOctave = constrain(currentOctave + dir, 1, 8);
    }
};