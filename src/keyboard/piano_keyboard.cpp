#include "piano_keyboard.h"

PianoKeyboard::PianoKeyboard() = default;

void PianoKeyboard::draw(Arduino_RGB_Display* gfx) {
    gfx->fillScreen(COLOR_BACKGROUND);

    // Header
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(2);
    gfx->setCursor(10, 10);
    gfx->print("ESP-8BIT SYNTH");

    drawControlPanel(gfx);
    drawWhiteKeys(gfx);
    drawBlackKeys(gfx);
}

int PianoKeyboard::getNoteAtTouch(int tx, int ty) {
    tx = PianoConfig::SCREEN_WIDTH - tx; // Flip X axis

    if (ty < PianoConfig::KEY_Y_OFFSET || ty > PianoConfig::KEY_Y_OFFSET + PianoConfig::WHITE_KEY_HEIGHT) {
        //TODO: Panel de Control (REC, PLAY, OCTAVE)
        return -1;
    }

    if (ty >= PianoConfig::KEY_Y_OFFSET && ty <= PianoConfig::KEY_Y_OFFSET + PianoConfig::BLACK_KEY_HEIGHT + 15) {
        int note = hitBlackKey(tx);
        if (note != -1) return note;
    }

    return hitWhiteKey(tx);
}

bool PianoKeyboard::isBlackKey(int index) const {
    for (int i = 0; i < PianoConfig::BLACK_KEY_COUNT; i++) {
        if (BLACK_KEY_INDICES[i] == index) return true;
    }
    return false;
}

void PianoKeyboard::drawWhiteKeys(Arduino_RGB_Display* gfx) {
    int whiteIdx = 0;
    for (int i = 0; i < 13; i++) {
        if (isBlackKey(i)) continue;

        int x = whiteIdx * PianoConfig::WHITE_KEY_WIDTH;
        int y = PianoConfig::KEY_Y_OFFSET;

        gfx->fillRect(x, y, PianoConfig::WHITE_KEY_WIDTH, PianoConfig::WHITE_KEY_HEIGHT, COLOR_WHITE_KEY);
        gfx->drawRect(x, y, PianoConfig::WHITE_KEY_WIDTH, PianoConfig::WHITE_KEY_HEIGHT, COLOR_BORDER);
        gfx->drawRect(x + 1, y + 1, PianoConfig::WHITE_KEY_WIDTH - 2, PianoConfig::WHITE_KEY_HEIGHT - 2, COLOR_BORDER);

        whiteIdx++;
    }
}

void PianoKeyboard::drawBlackKeys(Arduino_RGB_Display* gfx) {
    int whiteIdx = 0;
    for (int i = 0; i < 12; i++) {
        if (!isBlackKey(i)) {
            whiteIdx++;
            continue;
        }

        int x = (whiteIdx * PianoConfig::WHITE_KEY_WIDTH) - (PianoConfig::BLACK_KEY_WIDTH / 2);
        int y = PianoConfig::KEY_Y_OFFSET;

        gfx->fillRect(x, y, PianoConfig::BLACK_KEY_WIDTH, PianoConfig::BLACK_KEY_HEIGHT, COLOR_BLACK_KEY);
        gfx->drawRect(x, y, PianoConfig::BLACK_KEY_WIDTH, PianoConfig::BLACK_KEY_HEIGHT, COLOR_BORDER);
        gfx->drawRect(x + 1, y + 1, PianoConfig::BLACK_KEY_WIDTH - 2, PianoConfig::BLACK_KEY_HEIGHT - 2, COLOR_BORDER);
    }
}

int PianoKeyboard::hitBlackKey(int tx) const {
    int whiteIdx = 0;
    for (int i = 0; i < 12; i++) {
        if (!isBlackKey(i)) {
            whiteIdx++;
            continue;
        }
        int keyX = (whiteIdx * PianoConfig::WHITE_KEY_WIDTH) - (PianoConfig::BLACK_KEY_WIDTH / 2);
        if (tx >= keyX && tx <= keyX + PianoConfig::BLACK_KEY_WIDTH) {
            return i; // MIDI index: 1, 3, 6, 8, or 10
        }
    }
    return -1;
}

int PianoKeyboard::hitWhiteKey(int tx) const {
    static const int WHITE_KEY_MAP[] = {0, 2, 4, 5, 7, 9, 11, 12}; // C D E F G A B C

    int col = tx / PianoConfig::WHITE_KEY_WIDTH;
    if (col >= PianoConfig::WHITE_KEY_COUNT) col = PianoConfig::WHITE_KEY_COUNT - 1;
    return WHITE_KEY_MAP[col];
}

void PianoKeyboard::drawControlPanel(Arduino_RGB_Display* gfx) {
    int btnWidth = 80;
    int btnHeight = 60;
    int spacing = 16;

    int panelY = 35;
    int octavaActual = 1;
    int btnMinusX = 10;
    drawButton(gfx, btnMinusX, panelY, btnWidth, btnHeight, "-");

    int visorX = btnMinusX + btnWidth + spacing;
    drawButton(gfx, visorX, panelY, btnWidth, btnHeight, String(octavaActual).c_str());

    int btnPlusX = visorX + btnWidth + spacing;
    drawButton(gfx, btnPlusX, panelY, btnWidth, btnHeight, "+");

    int btnRecX = btnPlusX + btnWidth + (spacing * 3);
    drawButton(gfx, btnRecX, panelY, btnWidth, btnHeight, "REC");

    int btnPlayX = btnRecX + btnWidth + spacing;
    drawButton(gfx, btnPlayX, panelY, btnWidth, btnHeight, "PLAY");
}

void PianoKeyboard::drawButton(Arduino_RGB_Display* gfx, int x, int y, int w, int h, const char* label) {
    gfx->drawRect(x, y, w, h, COLOR_WHITE_KEY);

    gfx->setTextSize(3);
    gfx->setTextColor(COLOR_WHITE_KEY);

    int charCount = strlen(label);
    int textX = x + (w - (charCount * 6)) / 2;
    int textY = y + (h - 8) / 2;

    gfx->setCursor(textX, textY);
    gfx->print(label);
}

const int PianoKeyboard::BLACK_KEY_INDICES[PianoConfig::BLACK_KEY_COUNT] = {1, 3, 6, 8, 10};
