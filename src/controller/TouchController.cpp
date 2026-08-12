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
uint8_t TouchController::update() {
    touchSensor->read();

    if (touchSensor->isTouched) {
        int rawX = touchSensor->points[0].x;
        int rawY = touchSensor->points[0].y;
        int tx = mapTouchX(rawX);
        int ty = mapTouchY(rawY);

        // Debug: imprimir coordenadas de toque en el monitor serial
        Serial.print("Touch tx: ");
        Serial.print(tx);
        Serial.print("  ty: ");
        Serial.println(ty);

        return handleScreenTouch(tx, ty);
    }
    return 0;
}

uint8_t TouchController::handleScreenTouch(int tx, int ty) {
    
    // Check if the touch is within the top control bar area
    if (ty <= Config::BAR_HEIGHT && tx >= 0 && tx <= Config::SCREEN_WIDTH) {
        return controlsUi->handleTouch(tx, ty);
    }

    // Check if the touch is within the piano keyboard area
    if (ty < Config::KEY_Y_OFFSET || ty > Config::KEY_Y_OFFSET + Config::WHITE_KEY_HEIGHT) {
        uint8_t noteDetected = keyboard->handleTouch(tx, ty);
        midi->noteOn(1, noteDetected, 127);

        
        //midi->noteOff(1, lastNoteTouched, 0);

        return noteDetected;
    }

    return 0;
}