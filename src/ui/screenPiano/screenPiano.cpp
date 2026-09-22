#include "screenPiano.h"
#include "UIConfig.h"
#include "../UIEvent.h"

using namespace UIConfig;

ScreenPiano::ScreenPiano() {
    height = SCREEN_HEIGHT;
    width = SCREEN_WIDTH;
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

UIEvent ScreenPiano::handleTouch(int tx, int ty) {
    UIEvent event = { EventType::NONE, 0, 0 };

    // Check if the touch is within the top control bar area
    if (ty <= BAR_HEIGHT && tx >= 0 && tx <= SCREEN_WIDTH) {
        uint8_t control = controlsUi->handleTouch(tx, ty);        
        //TODO: Implementar eventos de control

        //...
    }

    // Check if the touch is within the piano keyboard area
    if (ty > BAR_HEIGHT && tx >= 0 && tx <= SCREEN_WIDTH) {
        keyboard->setOctave(controlsUi->currentOctave);
        uint8_t key = keyboard->handleTouch(tx, ty);
        event = { EventType::PLAY_NOTE, key, 127 };
    }

    return event;
}