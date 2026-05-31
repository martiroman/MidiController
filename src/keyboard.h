#pragma once
#include "piano-config.h"

#include <Arduino.h>

#define COLOR_BACKGROUND  0x0000
#define COLOR_WHITE_KEY   0xFFFF
#define COLOR_BLACK_KEY   0x10A2
#define COLOR_BORDER      0x0000

class PianoKeyboard {
public:
    static const int BLACK_KEY_INDICES[PianoConfig::BLACK_KEY_COUNT];

    int lastNotePlayed = -1;
    PianoKeyboard() {}

    
    void draw(Arduino_RGB_Display* gfx) {
        gfx->fillScreen(COLOR_BACKGROUND);

        // Header
        gfx->setTextColor(0xFFFF);
        gfx->setTextSize(2);
        gfx->setCursor(10, 10);
        gfx->print("HOLA ALMENDRA - ESP-8BIT SYNTH // OCTAVE 1");

        drawWhiteKeys(gfx);
        drawBlackKeys(gfx);
    }

    // Returns the MIDI note index (0–12), or -1 if no key was hit.
    int getNoteAtTouch(int tx, int ty) {
        tx = PianoConfig::SCREEN_WIDTH - tx; // Flip X axis

        if (ty < PianoConfig::KEY_Y_OFFSET || ty > PianoConfig::KEY_Y_OFFSET + PianoConfig::WHITE_KEY_HEIGHT) return -1;

        // 1. Check black keys first (upper zone + tolerance)
        if (ty >= PianoConfig::KEY_Y_OFFSET && ty <= PianoConfig::KEY_Y_OFFSET + PianoConfig::BLACK_KEY_HEIGHT + 15) {
            int note = hitBlackKey(tx);
            if (note != -1) return note;
        }

        // 2. Fall through to white key (linear mapping)
        return hitWhiteKey(tx);
    }

private:
    bool isBlackKey(int index) const {
        for (int i = 0; i < PianoConfig::BLACK_KEY_COUNT; i++)
            if (BLACK_KEY_INDICES[i] == index) return true;
        return false;
    }

    void drawWhiteKeys(Arduino_RGB_Display* gfx) {
        int whiteIdx = 0;
        for (int i = 0; i < 13; i++) {
            if (isBlackKey(i)) continue;

            int x = whiteIdx * PianoConfig::WHITE_KEY_WIDTH;
            int y = PianoConfig::KEY_Y_OFFSET;

            gfx->fillRect(x, y, PianoConfig::WHITE_KEY_WIDTH, PianoConfig::WHITE_KEY_HEIGHT, COLOR_WHITE_KEY);
            gfx->drawRect(x,     y,     PianoConfig::WHITE_KEY_WIDTH,     PianoConfig::WHITE_KEY_HEIGHT,     COLOR_BORDER);
            gfx->drawRect(x + 1, y + 1, PianoConfig::WHITE_KEY_WIDTH - 2, PianoConfig::WHITE_KEY_HEIGHT - 2, COLOR_BORDER);

            whiteIdx++;
        }
    }

    void drawBlackKeys(Arduino_RGB_Display* gfx) {
        int whiteIdx = 0;
        for (int i = 0; i < 12; i++) {
            if (!isBlackKey(i)) {
                whiteIdx++;
                continue;
            }

            int x = (whiteIdx * PianoConfig::WHITE_KEY_WIDTH) - (PianoConfig::BLACK_KEY_WIDTH / 2);
            int y = PianoConfig::KEY_Y_OFFSET;

            gfx->fillRect(x, y, PianoConfig::BLACK_KEY_WIDTH, PianoConfig::BLACK_KEY_HEIGHT, COLOR_BLACK_KEY);
            gfx->drawRect(x,     y,     PianoConfig::BLACK_KEY_WIDTH,     PianoConfig::BLACK_KEY_HEIGHT,     COLOR_BORDER);
            gfx->drawRect(x + 1, y + 1, PianoConfig::BLACK_KEY_WIDTH - 2, PianoConfig::BLACK_KEY_HEIGHT - 2, COLOR_BORDER);
        }
    }

    int hitBlackKey(int tx) const {
        int whiteIdx = 0;
        for (int i = 0; i < 12; i++) {
            if (!isBlackKey(i)) {
                whiteIdx++;
                continue;
            }
            int keyX = (whiteIdx * PianoConfig::WHITE_KEY_WIDTH) - (PianoConfig::BLACK_KEY_WIDTH / 2);
            if (tx >= keyX && tx <= keyX + PianoConfig::BLACK_KEY_WIDTH)
                return i; // MIDI index: 1, 3, 6, 8, or 10
        }
        return -1;
    }

    int hitWhiteKey(int tx) const {
        static const int WHITE_KEY_MAP[] = {0, 2, 4, 5, 7, 9, 11, 12}; // C D E F G A B C

        int col = tx / PianoConfig::WHITE_KEY_WIDTH;
        if (col >= PianoConfig::WHITE_KEY_COUNT) col = PianoConfig::WHITE_KEY_COUNT - 1;
        return WHITE_KEY_MAP[col];
    }
};

// Define static array outside the class
const int PianoKeyboard::BLACK_KEY_INDICES[PianoConfig::BLACK_KEY_COUNT] = {1, 3, 6, 8, 10};