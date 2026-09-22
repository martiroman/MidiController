#include <Arduino.h>
#include "UIController.h"
#include "../ui/Colors.h"
#include "../ui/screenPiano/UIConfig.h"
#include "../ui/screenPiano/keyboard/Notes.h"

using namespace UIConfig;

UIController::UIController(){
        introScreen = new IntroScreen();
        pianoScreen = nullptr;   // se crea en begin(), con el display ya inicializado
        currentScreen = introScreen;
    }

UIController::~UIController() {
        delete introScreen;
        delete pianoScreen;
}

void UIController::drawCurrentScreen(){
    if (currentScreen) {
        currentScreen->draw(gfx);
    }
}

UIEvent UIController::handleTouch(int tx, int ty) {
    if (!currentScreen) return { EventType::NONE, 0 , 0 };

    UIEvent event = currentScreen->handleTouch(tx, ty);
   
    if (event.type == EventType::START_PRESSED) {
        currentScreen = pianoScreen;
        drawCurrentScreen();
    }

    return event;
}

void UIController::debugMsg(const char* msg, uint16_t color){
    Serial.println(msg);

    if (!gfx) return;

    gfx->fillRect(DEBUG_X, DEBUG_Y, DEBUG_W, DEBUG_H, COLOR_DISPLAY_BG);

    gfx->setTextColor(color);
    gfx->setTextSize(DEBUG_TEXT_SIZE);
    gfx->setCursor(DEBUG_X + 4, DEBUG_Y + (DEBUG_H - (8 * DEBUG_TEXT_SIZE)) / 2);
    gfx->print(msg);
}

void UIController::begin(){
    auto *bus = new Arduino_ESP32RGBPanel(
        5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
        14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
    );
    gfx = new Arduino_RGB_Display(800, 480, bus);
    gfx->begin();
    gfx->fillScreen(BLACK);

    pianoScreen = new ScreenPiano();
}

void UIController::update(){
    
}