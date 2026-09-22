#include "TouchController.h"
#include "UIController.h"
#include "MidiController.h"
#include "../ui/screenPiano/UIConfig.h"
#include "../ui/screenPiano/keyboard/Notes.h"

using namespace UIConfig;

// initializing the touch sensor.
TouchController::TouchController()
    : touchSensor(8, 9, 4, -1, SCREEN_WIDTH, SCREEN_HEIGHT) {
    }

void TouchController::begin() {
    w = SCREEN_WIDTH;
    h = SCREEN_HEIGHT;

    touchSensor.begin();
    touchSensor.setResolution(w, h);
    touchSensor.setRotation(ROTATION_INVERTED);
}


void TouchController::update() {
    touchSensor.read();

    if (touchSensor.isTouched) {
        x = touchSensor.points[0].x;
        y = touchSensor.points[0].y;
        isTouched = true;
    } else {
        isTouched = false;
    }

}

bool TouchController::isTouchActive() {
    return isTouched;
}