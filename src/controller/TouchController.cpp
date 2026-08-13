#include "TouchController.h"
#include "../config/Config.h"
#include "../shared/hardware.h"

// Constructor for the TouchController class, initializing the touch sensor, controls UI, piano keyboard, and MIDI interface.
TouchController::TouchController()
    : touchSensor(8, 9, 4, -1, 800, 480) {
    }

void TouchController::begin() {
    touchSensor.begin();
    touchSensor.setResolution(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    touchSensor.setRotation(0);
}

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
    touchSensor.read();

    if (touchSensor.isTouched) {
        int rawX = touchSensor.points[0].x;
        int rawY = touchSensor.points[0].y;
        int tx = mapTouchX(rawX);
        int ty = mapTouchY(rawY);

        return handleScreenTouch(tx, ty);
    }
    return 0;
}

uint8_t TouchController::handleScreenTouch(int tx, int ty) {
    return screen->handleTouch(tx, ty);
}