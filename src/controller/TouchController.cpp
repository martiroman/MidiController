#include "TouchController.h"
#include "UIController.h"
#include "MidiController.h"
#include "../ui/screenPiano/UIConfig.h"
#include "../ui/screenPiano/keyboard/Notes.h"

using namespace UIConfig;

// Constructor for the TouchController class, initializing the touch sensor.
TouchController::TouchController()
    : touchSensor(8, 9, 4, -1, SCREEN_WIDTH, SCREEN_HEIGHT) {
    }

void TouchController::setUIController(UIController* controller) {
    ui = controller;
}

void TouchController::setMidiController(MidiController* controller) {
    midi = controller;
}

void TouchController::begin() {
    w = SCREEN_WIDTH;
    h = SCREEN_HEIGHT;

    touchSensor.begin();
    touchSensor.setResolution(w, h);
    touchSensor.setRotation(ROTATION_INVERTED);
}

// Maps the raw touch X coordinate to the screen coordinate system.
int TouchController::mapTouchX(int tx) {
    return tx;
}

// Maps the raw touch Y coordinate to the screen coordinate system.
int TouchController::mapTouchY(int ty) {
    return ty;
}

// Updates the touch controller state, handling touch input and MIDI note events.
uint8_t TouchController::update() {
    if (!ui) return 0;

    touchSensor.read();

    if (touchSensor.isTouched) {
        int rawX = touchSensor.points[0].x;
        int rawY = touchSensor.points[0].y;
        int tx = mapTouchX(rawX);
        int ty = mapTouchY(rawY);

        uint8_t result = ui->handleTouch(tx, ty);

        if (midi && ui->isPianoScreen() && result != NO_NOTE) {
            if (activeNote != result) {
                if (activeNote >= 0) midi->noteOff(activeNote, 0);
                midi->noteOn(result, 127);
                activeNote = result;
            }
        }

        return result;
    }

    if (midi && activeNote >= 0) {
        midi->noteOff(activeNote, 0);
        activeNote = -1;
    }

    return 0;
}
