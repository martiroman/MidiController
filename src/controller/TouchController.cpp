#include "TouchController.h"

// Constructor for the TouchController class, initializing the touch sensor, controls UI, piano keyboard, and MIDI interface.
TouchController::TouchController(TAMC_GT911* touchSensor, ControlsUI* controlsUi, PianoKeyboard* keyboard, UsbMidi* midi)
    : touchSensor(touchSensor), controlsUi(controlsUi), keyboard(keyboard), midi(midi) {}

// Maps the raw touch X coordinate to the screen coordinate system.
int TouchController::mapTouchX(int tx) {
    return Config::SCREEN_WIDTH - tx;
}

// Maps the raw touch Y coordinate to the screen coordinate system.
int TouchController::mapTouchY(int ty) {
    return Config::SCREEN_HEIGHT - ty;
}

// Updates the touch controller state, handling touch input and MIDI note events.
void TouchController::update() {
    touchSensor->read();

    if (touchSensor->isTouched) {
        int rawX = touchSensor->points[0].x;
        int rawY = touchSensor->points[0].y;
        int tx = mapTouchX(rawX);
        int ty = mapTouchY(rawY);

        handleScreenTouch(tx, ty);
        return;
    }

    releaseCurrentNote();
}

void TouchController::handleScreenTouch(int tx, int ty) {
    
    // Check if the touch is within the top control bar area
    if (ty <= Config::BAR_HEIGHT && tx >= 0 && tx <= Config::SCREEN_WIDTH) {
        if (controlsUi->handleTouch(tx, ty)) {
            releaseCurrentNote();
        }
        return;
    }

    // Check if the touch is within the piano keyboard area
    uint8_t noteDetected = -1;
    if (ty < Config::KEY_Y_OFFSET || ty > Config::KEY_Y_OFFSET + Config::WHITE_KEY_HEIGHT) {
        noteDetected = keyboard->handleTouch(tx, ty);
    }

    // If a note is detected, handle MIDI note-on and note-off events accordingly
    if (noteDetected != -1) {   
        if (noteDetected == lastNoteTouched) {
            return;
        }

        if (lastNoteTouched != -1) {
            midi->noteOff(1, lastNoteTouched, 0);
        }

        midi->noteOn(1, noteDetected, 127);
        lastNoteTouched = noteDetected;
    }
    return;
}

// Releases the currently pressed MIDI note, if any, by sending a note-off message and resetting the last touched note index.
void TouchController::releaseCurrentNote() {
    if (lastNoteTouched == -1) {
        return;
    }

    midi->noteOff(1, lastNoteTouched, 0);
    lastNoteTouched = -1;
}
