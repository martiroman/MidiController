#include "PianoKeyboard.h"
#include "Notes.h"
#include "../UIConfig.h"
#include "../../Colors.h"

using namespace UIConfig;

PianoKeyboard::PianoKeyboard() = default;

void PianoKeyboard::draw(Arduino_RGB_Display* gfx) {
    drawWhiteKeys(gfx);
    drawBlackKeys(gfx);
}

void PianoKeyboard::setOctave(int octave) {
    currentOctave = octave;
}

uint8_t PianoKeyboard::handleTouch(int tx, int ty) {
    if (ty < KEY_Y_OFFSET || ty > KEY_Y_OFFSET + WHITE_KEY_HEIGHT) {
        return NO_NOTE;
    }

    // Franja superior: ahi las negras se superponen a las blancas y tienen prioridad.
    // hitBlackKey devuelve -1 si no se toco ninguna, hay que filtrarlo ANTES de indexar.
    if (ty <= KEY_Y_OFFSET + BLACK_KEY_HEIGHT + 15) {
        int blackIdx = hitBlackKey(tx);
        if (blackIdx >= 0) {
            lastNotePlayed = getMidiNote(blackIdx, currentOctave);
            return lastNotePlayed;
        }
    }

    lastNotePlayed = getMidiNote(hitWhiteKey(tx), currentOctave);

    return lastNotePlayed;
}

bool PianoKeyboard::isBlackKey(int index) const {
    for (int i = 0; i < BLACK_KEY_COUNT; i++) {
        if (BLACK_KEY_INDICES[i] == index) return true;
    }
    return false;
}

void PianoKeyboard::drawWhiteKeys(Arduino_RGB_Display* gfx) {
    int whiteIdx = 0;
    for (int i = 0; i < 13; i++) {
        if (isBlackKey(i)) continue;

        int x = whiteIdx * WHITE_KEY_WIDTH;
        int y = KEY_Y_OFFSET;

        gfx->fillRect(x, y, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT, COLOR_WHITE_KEY);
        gfx->drawRect(x, y, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT, COLOR_BORDER);
        gfx->drawRect(x + 1, y + 1, WHITE_KEY_WIDTH - 2, WHITE_KEY_HEIGHT - 2, COLOR_BORDER);

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

        int x = (whiteIdx * WHITE_KEY_WIDTH) - (BLACK_KEY_WIDTH / 2);
        int y = KEY_Y_OFFSET;

        gfx->fillRect(x, y, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, COLOR_BLACK_KEY);
        gfx->drawRect(x, y, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, COLOR_BORDER);
        gfx->drawRect(x + 1, y + 1, BLACK_KEY_WIDTH - 2, BLACK_KEY_HEIGHT - 2, COLOR_BORDER);
    }
}

int PianoKeyboard::hitBlackKey(int tx) const {
    int whiteIdx = 0;
    for (int i = 0; i < 12; i++) {
        if (!isBlackKey(i)) {
            whiteIdx++;
            continue;
        }
        int keyX = (whiteIdx * WHITE_KEY_WIDTH) - (BLACK_KEY_WIDTH / 2);
        if (tx >= keyX && tx <= keyX + BLACK_KEY_WIDTH) {
            return i; // MIDI index: 1, 3, 6, 8, or 10
        }
    }
    return -1;
}

int PianoKeyboard::hitWhiteKey(int tx) const {
    static const int WHITE_KEY_MAP[] = {0, 2, 4, 5, 7, 9, 11, 12}; // C D E F G A B C

    int col = tx / WHITE_KEY_WIDTH;
    if (col >= WHITE_KEY_COUNT) col = WHITE_KEY_COUNT - 1;
    return WHITE_KEY_MAP[col];
}

