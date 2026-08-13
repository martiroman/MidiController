#include "screenPiano.h"

ScreenPiano::ScreenPiano() {
    controlsUi = new ControlsUI();
    keyboard = new PianoKeyboard();
}

ScreenPiano::~ScreenPiano() {
    delete controlsUi;
    delete keyboard;
}

void ScreenPiano::draw(Arduino_RGB_Display* gfx) {
    controlsUi->draw(gfx);
    keyboard->draw(gfx);
}

uint8_t ScreenPiano::handleTouch(int tx, int ty) {
    // Check if the touch is within the top control bar area
    if (ty <= Config::BAR_HEIGHT && tx >= 0 && tx <= Config::SCREEN_WIDTH) {
        return controlsUi->handleTouch(tx, ty);
    }

    // Check if the touch is within the piano keyboard area
    if (ty > Config::BAR_HEIGHT && tx >= 0 && tx <= Config::SCREEN_WIDTH) {
        return keyboard->handleTouch(tx, ty);;
    }

    return 0;
}