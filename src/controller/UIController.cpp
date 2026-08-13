#include "UIController.h"

UIController::UIController(){
        introScreen = new IntroScreen();
        currentScreen = introScreen;
    }

UIController::~UIController() {
        delete introScreen;
}

void UIController::drawCurrentScreen(){
    if (currentScreen) {
        currentScreen->draw(gfx);
    }
}

uint8_t UIController::handleTouch(int tx, int ty) {
    if (currentScreen) {
        return currentScreen->handleTouch(tx, ty);
    }
    return 0;
}

void UIController::begin(){
    auto *bus = new Arduino_ESP32RGBPanel(
        5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
        14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
    );
    auto *gfx = new Arduino_RGB_Display(800, 480, bus);
    gfx->begin();
    gfx->fillScreen(BLACK);
}